#include "eepch.h"

#include "Model.h"

#include <glm/glm.hpp>
#include "ElevateEngine/Renderer/Shader/Shader.h"
#include "ElevateEngine/Renderer/Shader/ShaderManager.h"
#include "ElevateEngine/Core/GameObject.h"

#include "ElevateEngine/Renderer/Renderer.h"

#include <ElevateEngine/Core/ComponentRegistry.h> // TODO REMOVE IF REGISTER_COMPONENT IS NOT USED ANYMORE
namespace Elevate
{
    REGISTER_COMPONENT(Model);
}

Elevate::Model::Model(PrimitiveType type) : Model("", nullptr)
{
    switch (type)
    {
    case PrimitiveType::Cube:
        m_batchedMesh = Mesh::GenerateCube(1.0f);
        break;
    case PrimitiveType::UVSphere:
        m_batchedMesh = Mesh::GenerateUVSphere(1.0f, 36, 18);
        break;
    case PrimitiveType::Cubesphere:
    case PrimitiveType::Icosphere:
    case PrimitiveType::Cylinder:
    case PrimitiveType::Capsule:
    case PrimitiveType::Plane:
        m_batchedMesh = Mesh::GeneratePlane(1.0f, 1);
        break;
    case PrimitiveType::Quad:
        m_batchedMesh = Mesh::GenerateQuad(1.0f);
        break;
    case PrimitiveType::Torus:
    default:
        EE_CORE_ASSERT(false, "Unsupported primitive shape given for mesh creation.");
        break;
    }
}

Elevate::Model::Model(std::string path, ShaderPtr shader, MaterialPtr material)
{
    SetShader(shader ? shader : ShaderManager::GetShader("default"));
    SetMaterial(material ? material : std::make_shared<Material>());

    if (!path.empty())
        LoadModel(path);
}

void Elevate::Model::LoadModel(std::string path)
{
    // Importing the scene
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_ImproveCacheLocality);

    // Error checking and exception catcher
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        EE_CORE_ERROR("ASSIMP LOADING ERROR : {0}", import.GetErrorString());
        return;
    }
    m_Directory = path.substr(0, path.find_last_of('/')); // Used to get the textures afterward

    // Recursive method to process all the nodes in the model
    MeshData data;
    std::vector<Mesh> meshes;
    ProcessNode(scene->mRootNode, scene, data);

    m_batchedMesh = Mesh(data);
}

void Elevate::Model::ProcessNode(aiNode* node, const aiScene* scene, MeshData& data)
{
    // process all the node's MESHES (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene, data);
    }
    // then do the same for each of its CHILDREN(S)
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, data); // Continue till we run out of childrens
    }
}

void Elevate::Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& data)
{
    uint32_t offset = (uint32_t) data.Vertices.size();

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        ExtractMeshVertex(mesh, vertex, i);
        data.Vertices.emplace_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            data.Indices.emplace_back(face.mIndices[j] + offset);
        }
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse, data);
        LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular, data);
    }
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

void Elevate::Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texType, MeshData& data)
{
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string path = str.C_Str();

        bool skip = false;
        for (TexturePtr tex : data.Textures)
        {
            if (tex->MatchesPath(path))
            {
                skip = true;
                break;
            }
        }
            
        if (!skip)
        {
            data.Textures.emplace_back(Texture::CreateFromFile(path, texType));
        }
    }
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
