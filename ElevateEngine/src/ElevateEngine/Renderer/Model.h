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
        COMPONENT_LAYOUT({});
        Model() = default;
        Model(PrimitiveType type);
        Model(std::string path);
        Model(std::string path, ShaderPtr shader);
        Model(std::string path, ShaderPtr shader, MaterialPtr material);

        // TODO : should change the shader in the renderer
        inline void SetShader(ShaderPtr newShader) { m_Shader = newShader; }
        inline void SetMaterial(MaterialPtr material) { m_Material = material; }

        void PreRender() override;
        void Render() override;

        const inline std::shared_ptr<Shader>& GetShader() const { return m_Shader; }
    private:
        void LoadModel(std::string path);
        void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        void ExtractMeshVertex(aiMesh* mesh, Vertex& vertex, int index);
        std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string );

    private:
        ShaderPtr m_Shader;
        MaterialPtr m_Material;
        // model data
        Mesh m_batchedMesh;
        std::string m_Directory;
    };
}