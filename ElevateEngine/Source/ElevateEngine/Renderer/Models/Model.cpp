module;

#include <entt/entt.hpp>

#include <mutex>
#include <string>
#include <memory>
#include <filesystem>
#include <glm/glm.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

module Elevate.Renderer.Models.Model;

// Foundations
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Assert;
import Elevate.Foundations.Paths;
import Elevate.Foundations.Services;
import Elevate.Foundations.Tasks;
// Core
import Elevate.Core.Application; // Used to get the DI services
// Renderer
import Elevate.Renderer.Renderer;
import Elevate.Renderer.Texture;
import Elevate.Renderer.Shaders;

namespace Elevate
{
	std::string GetUniformNameByType(std::shared_ptr<Texture> texture)
	{
		switch (texture->GetUsage())
		{
		case TextureType::Diffuse:			return "diffuseTex";
		case TextureType::Specular:		return "specularTex";
		case TextureType::Ambient:			return "ambientTex";
		case TextureType::AmbientOcclusion: return "aoTex";
		case TextureType::Normal:			return "normalTex";
		default: return "";
		}
	}

	Model::Model(PrimitiveType type, MaterialPtr material) : Model("", nullptr)
	{
		SetMaterial(material ? material : MaterialRegistry::GetMaterial(EE_DEFAULT_MATERIAL));

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
			Assert::That(false, "Unsupported primitive shape given for mesh creation.");
			break;
		}
	}

	Model::Model(std::string path, MaterialPtr material)
	{
		// todo : add a macro or const expression for the default value in asset maangers
		SetMaterial(material ? material : MaterialRegistry::GetMaterial(EE_DEFAULT_MATERIAL));

		if (!path.empty())
		{
			LoadModel(path);
		}
	}

	Model::~Model()
	{
		*m_alive = false;
	}

	void Model::SetMaterial(MaterialPtr material)
	{
		m_material = material ? material : MaterialRegistry::GetMaterial(EE_DEFAULT_MATERIAL);
		if (m_material)
		{
			for (auto& tex : m_batchedMesh.GetTextures())
			{
				m_material->SetTexture(GetUniformNameByType(tex), tex);
			}
		}
	}

	void Model::LoadModel(const std::string& path)
	{
		auto resolvedPath = PathResolver::Resolve(path);
		auto scheduler = Application::Get().GetServices().Resolve<ITaskScheduler>();
		std::weak_ptr<ITaskScheduler> weakScheduler = scheduler;

		scheduler->Submit(
			TaskDescriptor(TaskAffinity::Any, TaskPriority::Normal, "Model Loading"),
			[this, resolvedPath, weakScheduler, alive = m_alive]() mutable
			{
				MeshData data;
				if (!ParseModel(resolvedPath, data))
				{
					return;
				}

				auto scheduler = weakScheduler.lock();
				scheduler->Submit(
					TaskDescriptor(TaskAffinity::MainThread, TaskPriority::Normal, "Model Publish"),
					[this, alive, directory = resolvedPath.parent_path().string(), data = std::move(data)]() mutable
					{
						if (!*alive)
						{
							return;
						}

						PublishModel(std::move(directory), data);
					});
			}
		);
	}

	bool Model::ParseModel(const std::filesystem::path& path, MeshData& data)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path.string(),
			aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes |
			aiProcess_ImproveCacheLocality | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CoreLogger::Error("ASSIMP LOADING ERROR : {}", import.GetErrorString());
			return false;
		}

		ProcessNode(path.parent_path().string(), scene->mRootNode, scene, data);

		if (data.Vertices.empty() || data.Indices.empty())
		{
			CoreLogger::Error("Model '{}' contains no geometry.", path.string());
			return false;
		}
		return true;
	}

	void Model::ProcessNode(std::string basePath, aiNode* node, const aiScene* scene, MeshData& data)
	{
		// process all the node's MESHES (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(basePath, mesh, scene, data);
		}
		// then do the same for each of its CHILDREN(S)
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(basePath, node->mChildren[i], scene, data); // Continue till we run out of childrens
		}
	}

	void Model::ProcessMesh(std::string basePath, aiMesh* mesh, const aiScene* scene, MeshData& data)
	{
		uint32_t offset = (uint32_t)data.Vertices.size();

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

			LoadMaterialTextures(basePath, material, aiTextureType_DIFFUSE, TextureType::Diffuse, data);
			LoadMaterialTextures(basePath, material, aiTextureType_SPECULAR, TextureType::Specular, data);

			// Load ambiant occlusion
			LoadMaterialTextures(basePath, material, aiTextureType_AMBIENT_OCCLUSION, TextureType::AmbientOcclusion, data);
			LoadMaterialTextures(basePath, material, aiTextureType_LIGHTMAP, TextureType::AmbientOcclusion, data);
			LoadMaterialTextures(basePath, material, aiTextureType_AMBIENT, TextureType::AmbientOcclusion, data);

			// Normal map
			LoadMaterialTextures(basePath, material, aiTextureType_NORMALS, TextureType::Normal, data);
			LoadMaterialTextures(basePath, material, aiTextureType_HEIGHT, TextureType::Normal, data);
		}
	}

	void Model::ExtractMeshVertex(aiMesh* mesh, Vertex& vertex, int i)
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

	void Model::LoadMaterialTextures(std::string basePath, aiMaterial* mat, aiTextureType type, TextureType texType, MeshData& data)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			std::filesystem::path directory(basePath);
			directory.append(str.C_Str());
			std::string path = directory.string();

			bool skip = false;
			for (std::shared_ptr<Texture> tex : data.Textures)
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

	void Model::PublishModel(std::string directory, const MeshData& data)
	{
		m_Directory = std::move(directory);
		m_batchedMesh = Mesh(data);
		if (m_material)
		{
			for (auto& tex : m_batchedMesh.GetTextures())
			{
				m_material->SetTexture(GetUniformNameByType(tex), tex);
			}
		}
	}

	void Model::Render()
	{
		if (!m_material || !m_batchedMesh.GetVertexArray())
		{
			return; // Nothing to draw
		}

		Renderer::SubmitMesh(m_batchedMesh.GetVertexArray(), m_material, gameObject->GetModelMatrix());
	}
}
