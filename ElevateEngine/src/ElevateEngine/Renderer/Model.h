#pragma once
#include <vector>
#include <string>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "ElevateEngine/Core/Component.h"
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Renderer/RenderState.h>
#include "Mesh.h"

namespace Elevate
{
    class Model : public Component
    {
    public:
        COMPONENT_LAYOUT({});
        Model() = default;
        Model(PrimitiveType type);
        Model(std::string path, ShaderPtr shader = nullptr, MaterialPtr material = nullptr);

        // TODO : should change the shader in the renderer
        inline void SetShader(ShaderPtr newShader) { m_Shader = newShader; }
        inline void SetMaterial(MaterialPtr material) { m_Material = material; }

        void PreRender() override;
        void Render() override;

        const inline std::shared_ptr<Shader>& GetShader() const { return m_Shader; }

        RenderState& GetRenderState() { return m_attributes; }
    private:
        void LoadModel(std::string path);
        void ProcessNode(aiNode* node, const aiScene* scene, MeshData& data);
        void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& data);
        void ExtractMeshVertex(aiMesh* mesh, Vertex& vertex, int index);
        void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texType, MeshData& data);
    private:
        ShaderPtr m_Shader;
        MaterialPtr m_Material;
        // model data
        Mesh m_batchedMesh;
        std::string m_Directory;
        RenderState m_attributes;
    };
}