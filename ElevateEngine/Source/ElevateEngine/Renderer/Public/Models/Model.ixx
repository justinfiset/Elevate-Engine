module;

#include <vector>
#include <string>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Reflection.h"

export module Elevate.Renderer.Models.Model;

import Elevate.Core.Scene;
import Elevate.Renderer.Texture;
import Elevate.Renderer.Buffers;
import Elevate.Renderer.Materials.Material;
import Elevate.Renderer.Meshes.Mesh;
import Elevate.Renderer.Vertex;

export namespace Elevate
{
	class Model : public Component
	{
	public:
		BEGIN_COMPONENT(Model);
		EECATEGORY("Rendering");

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
		EEObjectPtr<Material> m_material;
		PROPERTY(m_material);

		Mesh m_batchedMesh;
		std::string m_Directory;

		END_COMPONENT();
	};
}