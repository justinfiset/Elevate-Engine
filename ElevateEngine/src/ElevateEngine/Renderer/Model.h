#pragma once
#include <vector>
#include <string>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Texture.h"
#include "Mesh.h"

namespace Elevate
{
    class Model
    {
    public:
        Model(std::string path);
        // TODO move somewhere else // PUT PRIVATE;
        inline const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
        void Draw(std::shared_ptr<Shader> shader, glm::mat4 modelMatrix);

        //inline glm::mat4& GetMatrix() const { return *m_ModelMatrix; }
        //inline void SetMatrix(const glm::mat4& newMatrix) const { *m_ModelMatrix = newMatrix; }
    private:
        void LoadModel(std::string path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    private:
        // model data
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;
        //std::unique_ptr<glm::mat4> m_ModelMatrix;
        std::vector<std::shared_ptr<Texture>> textures_loaded;
    };
}