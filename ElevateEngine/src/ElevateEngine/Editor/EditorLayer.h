#pragma once

#ifdef EE_EDITOR_BUILD

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Assert.h>
#include <ElevateEngine/Core/Layers/Layer.h>
#include <ElevateEngine/Core/Command.h>

#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Editor/Panels/ScenePanel.h>
#include <ElevateEngine/Editor/Panels/HierarchyPanel.h>
#include <ElevateEngine/Editor/Panels/AnalyserPanel.h>
#include <ElevateEngine/Editor/Panels/AssetBrowserPanel.h>
#include <ElevateEngine/Editor/Panels/StatisticsPanel.h>

#include <memory>

#ifdef EE_USES_WWISE
#include <ElevateEngine/Audio/Wwise/Editor/WwiseBrowserWidget.h>
#endif // #ifdef EE_USES_WWISE

namespace Elevate
{
	class Shader;
	class GameObject;
	class Scene;
	class EditorCamera;
	class Texture;
}

namespace Elevate::Editor
{
	class EditorLayer : public Layer, public CommandManager
	{
	public:
		EditorLayer();

		void InitUI()
		{
			CreateWidget<HierarchyPanel>();
			CreateWidget<ScenePanel>();
			CreateWidget<AnalyserPanel>();
			CreateWidget<AssetBrowserPanel>();
			CreateWidget<StatisticsPanel>();

#ifdef EE_USES_WWISE
			CreateWidget<WwiseBrowserWidget>();
#endif // #ifdef EE_USES_WWISE
		}

		virtual void OnAttach() override;
		//virtual void OnDetach() {}
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		EditorCamera* GetCamera();
		inline std::weak_ptr<Scene> GetScene() { return m_EditorScene; }
		inline std::weak_ptr<GameObject> GetSelectedObject() { return m_SelectedObject; }
		void SelectObject(const EEObjectPtr<GameObject>& newSelection);
		
		inline static EditorLayer& Get() { return *s_Instance; }

		template <typename T>
		inline static void CreateWidget() 
		{
			bool isValid = std::is_base_of_v<EditorWidget, T>;
			EE_ASSERT(isValid, "An attemp to add a non widget element to the widget stack was detected.");
			Get().m_widgets.push_back(std::make_unique<T>());
		}
	private:
		std::vector<std::unique_ptr<EditorWidget>> m_widgets;
		std::shared_ptr<Texture> m_playTexture;
		std::shared_ptr<Texture> m_pauseTexture;
		std::shared_ptr<Texture> m_stopTexture;
		std::shared_ptr<Texture> m_coloredStopTexture;

		std::shared_ptr<GameObject> m_CameraObject;

		std::weak_ptr<GameObject> m_SelectedObject;

		// Editor Scene
		std::shared_ptr<Scene> m_EditorScene;
		
		// Grid
		std::shared_ptr<Shader> m_GridShader;
		std::shared_ptr<GameObject> m_GridObject;

		static EditorLayer* s_Instance;
	};
}

#endif // EE_EDITOR_BUILD