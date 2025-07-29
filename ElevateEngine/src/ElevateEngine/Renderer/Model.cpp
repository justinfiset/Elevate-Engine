#include "eepch.h"

#include "Model.h"

#include <glm/glm.hpp>
#include "ElevateEngine/Renderer/Shader/Shader.h"
#include "ElevateEngine/Renderer/Shader/ShaderManager.h"
#include "ElevateEngine/Core/GameObject.h"

#include "ElevateEngine/Renderer/Renderer.h"

namespace Elevate
{
    REGISTER_COMPONENT(Model);
}

Elevate::Model::Model(PrimitiveType type) : Model("", nullptr)
{
    Mesh mesh;

    switch (type)
    {
    case PrimitiveType::Cube:
        mesh = Mesh::GenerateCube(1.0f);
        break;
    case PrimitiveType::UVSphere:
        mesh = Mesh::GenerateUVSphere(1.0f, 36, 18);
        break;
    case PrimitiveType::Cubesphere:
    case PrimitiveType::Icosphere:
    case PrimitiveType::Cylinder:
    case PrimitiveType::Capsule:
    case PrimitiveType::Plane:
        mesh = Mesh::GeneratePlane(1.0f, 1);
        break;
    case PrimitiveType::Quad:
        mesh = Mesh::GenerateQuad(1.0f);
        break;
    case PrimitiveType::Torus:
    default:
        EE_CORE_ASSERT(false, "Unsupported primitive shape given for mesh creation.");
        break;
    }

    m_batchedMesh = mesh;
}

Elevate::Model::Model(std::string path) : Model(path, nullptr) { }

Elevate::Model::Model(std::string path, ShaderPtr shader) : Model(path, shader, nullptr) { }

Elevate::Model::Model(std::string path, ShaderPtr shader, MaterialPtr material)
{
    if (shader)
    {
        SetShader(shader);
    }
    else
    {
        SetShader(ShaderManager::GetShader("default"));
    }

    if (material)
    {
        SetMaterial(material);
    }
    else 
    {
        // TODO GET A DEFAULT PTR FROM SOMEWHERE LIKE A MATERIAL MANAGER OR LIBRARY
        SetMaterial(std::make_shared<Material>());
    }

    if (path.length() > 0)
    {
        LoadModel(path);
    }
}

void Elevate::Model::LoadModel(std::string path)
{
    // Importing the scene
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

    // Error checking and exception catcher
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        EE_CORE_ERROR("ASSIMP LOADING ERROR : {0}", import.GetErrorString());
        return;
    }
    m_Directory = path.substr(0, path.find_last_of('/')); // Used to get the textures afterward

    // Recursive method to process all the nodes in the model
    std::vector<Mesh> meshes;
    ProcessNode(scene->mRootNode, scene, meshes);

    if (!meshes.empty()) 
    {
        m_batchedMesh = Mesh::CombineMeshes(meshes);
    }
    else 
    {
        EE_CORE_WARN("(Model loader) : No meshes found in node : {}.", path);
    }

    Renderer::SubmitModel(*this);
}

void Elevate::Model::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes)
{
    // process all the node's MESHES (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its CHILDREN(S)
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, meshes); // Continue till we run out of childrens
    }
}

Elevate::Mesh Elevate::Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        ExtractMeshVertex(mesh, vertex, i);
        vertices.push_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<std::shared_ptr<Texture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

void Elevate::Model::ExtractMeshVertex(aiMesh* mesh, Vertex& vertex, int i)
{
    // process vertex positions, normals and texture coordinates
    glm::vec3 vector;
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.Position = vector;

    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.Normal = vector;

    if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
    {
        // text coords
        glm::vec2 coords;
        coords.x = mesh->mTextureCoords[0][i].x;
        coords.y = mesh->mTextureCoords[0][i].y;
        vertex.TexCoords = coords;
    }

    if (mesh->mTangents)
    {
        // tangent
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.Tangent = vector;
    }

    if (mesh->mBitangents)
    {
        // bitangent
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.Bitangent = vector;
    }
}

std::vector<std::shared_ptr<Elevate::Texture>> Elevate::Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texType)
{
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string path = str.C_Str();
        size_t lastSlash = path.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            path = path.substr(lastSlash + 1);
        }

        bool skip = false;
        for (unsigned int j = 0; j < textures.size(); j++)
        {
            if (textures[j]->MatchesPath(path)) {
                textures.push_back(textures[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            TexturePtr texture = Texture::CreateFromFile(path, texType);
            textures.push_back(texture);
        }
    }
    return textures;
}

void Elevate::Model::PreRender()
{
    Renderer::SubmitShaderForSetup(m_Shader);
}

void Elevate::Model::Render()
{
    // TODO send to render comment
    Renderer::PushRenderState(m_attributes);
    m_Shader->Bind();
    m_Shader->UseMaterial(m_Material);
    m_Shader->SetModelMatrix(*gameObject);
    m_batchedMesh.Draw(m_Shader);
}
