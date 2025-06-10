#pragma once
#include <ElevateEngine/Editor/EditorWidget.h>

#include "imgui.h"
#include "tinyfiledialogs.h"

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
				const char* filters[] = { "*.sky" };
				const char* filePath = tinyfd_openFileDialog(
					"Select a skybox file",
					"",
					1,
					filters,
					"*.sky - Elevate Skybox File",
					0
				);

				if (filePath)
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