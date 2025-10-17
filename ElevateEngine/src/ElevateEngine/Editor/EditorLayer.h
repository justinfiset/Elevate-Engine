#pragma once

#include <ElevateEngine/Core/Layers/Layer.h>
#include <ElevateEngine/Core/Command.h>
#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Core/Assert.h>

#include <memory>

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

		void InitUI();

		virtual void OnAttach() override;
		//virtual void OnDetach() {}
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		EditorCamera* GetCamera();
		inline std::weak_ptr<Scene> GetScene() { return m_EditorScene; }
		inline std::weak_ptr<GameObject> GetSelectedObject() { return m_SelectedObject; }
		void SelectObject(std::shared_ptr<GameObject> newSelection);
		
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