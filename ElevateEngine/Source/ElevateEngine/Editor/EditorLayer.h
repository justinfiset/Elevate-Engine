#pragma once

#ifdef EE_EDITOR_BUILD

#include <memory>
#include <type_traits>
#include <vector>

#include <ElevateEngine/Editor/Commands/Command.h>
#include <ElevateEngine/Editor/EditorWidget.h>

#ifdef EE_USES_WWISE
#include <ElevateEngine/Audio/Wwise/Editor/WwiseBrowserWidget.h>
#endif // #ifdef EE_USES_WWISE

import Elevate.Core.Layers.Layer;
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;

namespace Elevate
{
	class Event;
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
		inline std::weak_ptr<EEObject> GetSelectedObject() { return m_SelectedObject; }
		void SelectObject(const EEObjectPtr<EEObject>& newSelection);

		inline static EditorLayer& Get() { return *s_Instance; }

		template <typename T>
		inline static void CreateWidget()
		{
			bool isValid = std::is_base_of_v<EditorWidget, T>;
			Assert::That(isValid, "An attemp to add a non widget element to the widget stack was detected.");
			Get().m_widgets.push_back(std::make_unique<T>());
		}
	private:
		std::vector<std::unique_ptr<EditorWidget>> m_widgets;
		std::shared_ptr<Texture> m_playTexture;
		std::shared_ptr<Texture> m_pauseTexture;
		std::shared_ptr<Texture> m_stopTexture;
		std::shared_ptr<Texture> m_coloredStopTexture;

		std::shared_ptr<GameObject> m_CameraObject;

		std::weak_ptr<EEObject> m_SelectedObject;

		// Editor Scene
		std::shared_ptr<Scene> m_EditorScene;

		// Grid
		std::shared_ptr<Shader> m_GridShader;
		std::shared_ptr<GameObject> m_GridObject;

		static EditorLayer* s_Instance;
	};
}

#endif // EE_EDITOR_BUILD