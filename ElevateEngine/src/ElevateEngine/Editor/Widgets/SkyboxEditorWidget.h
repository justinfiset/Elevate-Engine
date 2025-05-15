#pragma once
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
			auto skybox = scene->GetSkybox();
			if (scene)
			{
				ImGui::Text("Skybox : %s", scene->GetSkybox().lock()->GetFilePath().c_str());
			}
			else
			{
				ImGui::Text("Skybox : (none)");
			}

			if (ImGui::Button("Select Skybox File"))
			{

			}
			ImGui::End();
		}

	private:
	};
}