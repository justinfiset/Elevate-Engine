#pragma once
#include <memory>

#include <ElevateEngine/Core/Layers/Layer.h>
#include <ElevateEngine/Core/GameObject.h>

#include <ElevateEngine/Renderer/Camera.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>

#include <ElevateEngine/Editor/Panels/ScenePanel.h>
#include <ElevateEngine/Editor/Panels/HierarchyPanel.h>
#include <ElevateEngine/Editor/Panels/AnalyserPanel.h>
#include <ElevateEngine/Editor/Camera/EditorCamera.h>
#include <ElevateEngine/Editor/Panels/StatisticsPanel.h>
#include <ElevateEngine/Editor/Panels/AssetBrowserPanel.h>
#include <ElevateEngine/Editor/EditorWidget.h>

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

		// Panels
		std::unique_ptr<ScenePanel> m_ScenePanel;
		std::unique_ptr<HierarchyPanel> m_HierarchyPanel;
		std::unique_ptr<AnalyserPanel> m_AnalyserPanel;
		std::unique_ptr<StatisticsPanel> m_StatisticsPanel;
		std::unique_ptr<AssetBrowserPanel> m_AssetBrowserPanel;

		// TODO set dans une autre classe
		ScenePtr m_EditorScene;
		// Grid
		ShaderPtr m_GridShader;
		GameObjectPtr m_GridObject;

		static EditorLayer* s_Instance;
	};
}