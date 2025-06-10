#pragma once
#include <memory>

#include <ElevateEngine/Core/Layers/Layer.h>
#include <ElevateEngine/Core/GameObject.h>

#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Editor/Camera/EditorCamera.h>

namespace Elevate::Editor
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		void InitUI();

		virtual void OnAttach() override;
		//virtual void OnDetach() {}
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		inline EditorCamera* GetCamera() { return m_CameraObject->GetComponent<EditorCamera>(); }
		inline ScenePtr GetScene() { return m_EditorScene; }
		inline GameObjectPtr GetSelectedObject() { return m_SelectedObject; }
		void SelectObject(GameObjectPtr newSelection);
		
		inline static EditorLayer& Get() { return *s_Instance; }

		template <typename T>
		inline static void CreateWidget() 
		{
			bool isValid = std::is_base_of_v<EditorWidget, T>;
			EE_CORE_ASSERT(isValid, "An attemp to add a non widget element to the widget stack was detected.");
			Get().m_widgets.push_back(std::make_unique<T>());
		}
	private:
		std::vector<std::unique_ptr<EditorWidget>> m_widgets;

		Elevate::GameObjectPtr m_CameraObject;

		GameObjectPtr m_SelectedObject;

		// Editor Scene
		ScenePtr m_EditorScene;

		// Grid
		ShaderPtr m_GridShader;
		GameObjectPtr m_GridObject;

		static EditorLayer* s_Instance;
	};
}