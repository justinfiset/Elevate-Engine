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
			ImGui::Text("Skybox : " + )
			if(ImGui::Button("Select Skybox File"))

			ImGui::End();
		}

	private:
	};
}