module;

#include <memory>

export module Elevate.Core.Scene.SceneLayer;

import Elevate.Core.Layers.Layer;

namespace Elevate
{
	class Camera;
	class Scene;
}

export namespace Elevate
{
	/// <summary>
	/// Simple layer thats contains a single scene and take care of all loading/unloading update and rendering.
	/// This is a good starting point if your are looking to create your own scene logic.
	/// </summary>
	class SceneLayer : public Layer
	{
	public:
		SceneLayer(std::shared_ptr<Scene> scene)
			: m_scene(scene) {
		}

		//virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		void OnRender(Camera* cam);
		//virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	protected:
		std::shared_ptr<Scene> m_scene;
	};
}