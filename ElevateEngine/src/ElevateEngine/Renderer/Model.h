#pragma once
#include <vector>
#include <string>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/ComponentRegistry.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Renderer/Material.h>
#include <ElevateEngine/Renderer/Mesh.h>
#include <ElevateEngine/Renderer/Buffer.h>

namespace Elevate
{
	class Model : public Component
	{
	public:
		BEGIN_COMPONENT(Model)
		EECATEGORY("Rendering")

		Model() = default;
		Model(PrimitiveType type);
		Model(std::string path, MaterialPtr material = nullptr);

		void SetMaterial(MaterialPtr material) { m_material = material; }

		void Render() override;

		// todo change this to material and also make a setter	
		//const inline std::shared_ptr<Shader>& GetShader() const { return m_shader; }

		RenderState& GetRenderState() { return m_attributes; }

		//virtual Component* Clone() override;
	private:
		void LoadModel(std::string path);
		void ProcessNode(std::string basePath, aiNode* node, const aiScene* scene, MeshData& data);
		void ProcessMesh(std::string basePath, aiMesh* mesh, const aiScene* scene, MeshData& data);
		void ExtractMeshVertex(aiMesh* mesh, Vertex& vertex, int index);
		void LoadMaterialTextures(std::string basePath, aiMaterial* mat, aiTextureType type, TextureType texType, MeshData& data);
	private:
		MaterialPtr m_material;
		// model data
		Mesh m_batchedMesh;
		std::string m_Directory;
		RenderState m_attributes;

		END_COMPONENT()
	};
}