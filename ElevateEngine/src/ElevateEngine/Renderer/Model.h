#pragma once
#include <vector>
#include <string>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "ElevateEngine/Core/Component.h"
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include "Mesh.h"

namespace Elevate
{
    class Model : public Component
    {
    public:
        COMPONENT_LAYOUT("Model", {});

        Model() = default;
        Model(std::string path);
        Model(std::string path, ShaderPtr shader);
        Model(std::string path, ShaderPtr shader, MaterialPtr material);
        // TODO move somewhere else // PUT PRIVATE;
        inline const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }

        // TODO : should change the shader in the renderer
        inline void SetShader(ShaderPtr newShader) { m_Shader = newShader; }
        inline void SetMaterial(MaterialPtr material) { m_Material = material; }

        // TODO REMOVE IF NOT NEEDED ANYMORE
        //inline glm::mat4& GetMatrix() const { return *m_ModelMatrix; }
        //inline void SetMatrix(const glm::mat4& newMatrix) const { *m_ModelMatrix = newMatrix; }
        void Init() override {}
        void Render() override;

        const inline std::shared_ptr<Shader>& GetShader() const { return m_Shader; }
    private:
        void LoadModel(std::string path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        void ExtractMeshVertex(aiMesh* mesh, Vertex& vertex, int index);
        std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string 
        );

    private:
        ShaderPtr m_Shader;
        MaterialPtr m_Material;
        // model data
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;
    };
}