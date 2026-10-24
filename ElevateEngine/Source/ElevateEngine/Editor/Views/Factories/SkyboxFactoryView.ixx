#pragma once

#include <ElevateEngine/Renderer/Environment/Cubemap/Cubemap.h>

#include "imgui.h"

// Foundations
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Files.FileDialog;
// Core
import Elevate.Core.Scene;
// Editor
import Elevate.Editor.Widgets.Widget;

namespace Elevate::Editor
{
	class SkyboxEditorWidget : public Widget
	{
	public:
		void OnImGuiRender() override
		{
			ImGui::Begin("Environment");
			ImGui::SeparatorText("Skybox");

			auto scene = SceneManager::GetCurrentScene(SceneType::RuntimeScene);
			auto skybox = scene->GetSkybox().lock();

			if (ImGui::Button("Select Skybox File"))
			{
				FileDialog::RequestOpenFile(
					"Select a skybox file",
					"",
					{ "*.sky" },
					"*.sky - Elevate Skybox File",
					false
				);
			}

			std::string filePath;
			if (FileDialog::DisplayAndGetResult(filePath))
			{
				CoreLogger::Trace("Setting new scene skybox from file : {}", filePath);
				if (!filePath.empty())
				{
					scene->SetSkybox(filePath);
				}
			}

			if (scene && skybox)
			{
				ImGui::Text("Skybox : %s", skybox->GetFilePath().c_str());
				// Skybox textures preview
				for (int i = 0; i < 6; i++)
				{

				}
			}
			else
			{
				ImGui::Text("Skybox : (none)");
			}

			ImGui::End();
		}
	};
}