module;

// GCC 15's module early-alias pass ICEs on implicit memcpy declarations
// streamed from imported BMIs unless <cstring> is visible first.
#include <cstring>
#include <memory>
#include <glm/glm.hpp>

module Elevate.Editor.Renderer;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Services;

import Elevate.Core.Application;
import Elevate.Core.Scene;
import Elevate.Core.Objects.ObjectPtr;

import Elevate.Renderer.Renderer;
import Elevate.Renderer.Shaders;
import Elevate.Renderer.Texture;
import Elevate.Renderer.Meshes.Mesh;
import Elevate.Renderer.Materials.Material;
import Elevate.Renderer.RenderState;
import Elevate.Renderer.Commands.RenderBucketType;

namespace Elevate
{
	void EditorRenderer::DrawBillboard(const glm::vec3& position, std::shared_ptr<Texture> icon, float scale, glm::vec4 colorModifier)
	{
		if (!icon) return;

		static Mesh billboardQuad = Mesh::GenerateQuad();
		static EEObjectPtr<Shader> billboardShader = ShaderManager::GetShader("editor/billboard");

		if (!billboardShader)
		{
			CoreLogger::Error("Could not find billboard shader 'editor/billboard'.");
			return;
		}

		MaterialPtr instanceMaterial = MaterialRegistry::LoadMaterial(billboardShader);

		RenderState state;
		state.BlendMode = BlendModeType::Alpha;
		state.CullMode = CullFace::None;
		state.DepthTest = false;
		state.DepthWrite = false;
		state.DepthFunc = DepthFunction::LessEqual;

		instanceMaterial->SetRenderState(state);
		instanceMaterial->SetBucket(RenderBucketType::Editor);

		instanceMaterial->SetTexture("billboardTexture", icon);
		instanceMaterial->Set("colorModifier", colorModifier);
		instanceMaterial->Set("worldPos", position);
		instanceMaterial->Set("scale", scale);

		Renderer::SubmitMesh(billboardQuad.GetVertexArray(), instanceMaterial, glm::mat4(1.0f));
	}
}
