module;

#include <memory>
#include <type_traits>
#include <vector>

export module Elevate.Editor.Layer;

#ifdef EE_EDITOR_BUILD

// Foundations
import Elevate.Foundations.Assert;
// Core
import Elevate.Core.Events.Event;
import Elevate.Core.Layers.Layer;
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Scene;
// Renderer
import Elevate.Renderer.Texture;
// Editor
import Elevate.Editor.Commands.Command;
import Elevate.Editor.Widgets.Widget;
import Elevate.Editor.Camera;

#ifdef EE_USES_WWISE
import Elevate.Editor.Panels.Audio.WwiseBrowser;
#endif // #ifdef EE_USES_WWISE

namespace Elevate
{
	class Shader;
}

export namespace Elevate::Editor
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
			bool isValid = std::is_base_of_v<Widget, T>;
			Assert::That(isValid, "An attemp to add a non widget element to the widget stack was detected.");
			Get().m_widgets.push_back(std::make_unique<T>());
		}

	private:
		std::shared_ptr<SceneManager> m_sceneManager;

		std::vector<std::unique_ptr<Widget>> m_widgets;
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

		bool m_themeIsSet = false;

		static EditorLayer* s_Instance;
	};
}

#endif // EE_EDITOR_BUILD