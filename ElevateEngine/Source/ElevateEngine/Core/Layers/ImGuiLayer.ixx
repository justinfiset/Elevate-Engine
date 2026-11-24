module;


export module Elevate.Core.Layers.ImGuiLayer;

import Elevate.Core.Layers.Layer;

export namespace Elevate
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		void PreRender();
		void Begin();
		void Render();
		void End();
		void Cleanup();

	private:
		float m_Time = 0.0f;
	};
}
