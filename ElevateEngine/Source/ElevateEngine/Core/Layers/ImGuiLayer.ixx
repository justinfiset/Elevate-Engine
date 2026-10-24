export module Elevate.Core.Layers.ImGuiLayer;

import Elevate.Core.Layers.Layer;

export namespace Elevate
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void PreRender();
		void Begin();
		void Render();
		void End();
		void Cleanup();

	private:
		float m_Time = 0.0f;
	};
}
