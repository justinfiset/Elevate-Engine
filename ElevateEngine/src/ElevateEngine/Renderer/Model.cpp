#include "eepch.h"

#include "Model.h"

#include <glm/glm.hpp>
#include "ElevateEngine/Renderer/Shader.h"

Elevate::Model::Model(std::string path)
{
    //m_ModelMatrix = std::make_unique<glm::mat4>(1.0f);
    LoadModel(path);
}

void Elevate::Model::Draw(std::shared_ptr<Shader> shader, glm::mat4 modelMatrix)
{
    // todo enlever d'ici
    shader->SetUniformMatrix4fv("model", modelMatrix); // set the model matrix
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
        m_Meshes[i].Draw(shader);
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
    ProcessNode(scene->mRootNode, scene);
}

void Elevate::Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's MESHES (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its CHILDREN(S)
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene); // Continue till we run out of childrens
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
        // process vertex positions, normals and texture coordinates

        // TODO PUT IN ANOTHER METHOD ExtractMeshVertex(mesh, index)
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
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // todo add tangeant
            // todo add bitangent
            // tangent
            //vector.x = mesh->mTangents[i].x;
            //vector.y = mesh->mTangents[i].y;
            //vector.z = mesh->mTangents[i].z;
            //vertex.Tangent = vector;
            //// bitangent
            //vector.x = mesh->mBitangents[i].x;
            //vector.y = mesh->mBitangents[i].y;
            //vector.z = mesh->mBitangents[i].z;
            //vertex.Bitangent = vector;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
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
        std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "material.diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<std::shared_ptr<Texture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "material.specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<std::shared_ptr<Elevate::Texture>> Elevate::Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<std::shared_ptr<Texture>> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j]->GetPath().data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            std::shared_ptr<Texture> texture;
            
            // We get the local path of the textures -> from abolute to relative
            std::string path = str.C_Str();
            size_t lastSlash = path.find_last_of("/\\");
            if (lastSlash != std::string::npos) {
                path = path.substr(lastSlash + 1);
            }

            texture.reset(Texture::Create(path));
            texture->SetType(typeName);
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}
