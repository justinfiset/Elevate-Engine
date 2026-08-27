#pragma once
#include <vector>
#include <string>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/TypeRegistry.h>
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
		Model(PrimitiveType type, MaterialPtr material = nullptr);
		Model(std::string path, MaterialPtr material = nullptr);

		void SetMaterial(MaterialPtr material);

		void Render() override;

	private:
		void LoadModel(std::string path);
		void ProcessNode(std::string basePath, aiNode* node, const aiScene* scene, MeshData& data);
		void ProcessMesh(std::string basePath, aiMesh* mesh, const aiScene* scene, MeshData& data);
		void ExtractMeshVertex(aiMesh* mesh, Vertex& vertex, int index);
		void LoadMaterialTextures(std::string basePath, aiMaterial* mat, aiTextureType type, TextureType texType, MeshData& data);

	private:
		MaterialPtr m_material;
		PROPERTY(m_material);

		Mesh m_batchedMesh;
		std::string m_Directory;

		END_COMPONENT()
	};
}