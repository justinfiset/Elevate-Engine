#pragma once
#include <ElevateEngine/Files/FileDialog.h>
#include <ElevateEngine/Scene/SceneManager.h>
#include <ElevateEngine/Editor/EditorWidget.h>

#include "imgui.h"

namespace Elevate::Editor
{
	class SkyboxEditorWidget : public EditorWidget
	{
	public:
		void OnImGuiRender() override 
		{
			ImGui::Begin("Environment");
			ImGui::SeparatorText("Skybox");
			
			auto scene = SceneManager::GetCurrentScene(RuntimeScene);
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
				EE_CORE_TRACE("Setting new scene skybox from file : {}", filePath);
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