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
		void OnImGuiRenderBegin() override;
		void OnImGuiRenderEnd() override;
		void Cleanup() override;

	private:
		float m_Time = 0.0f;
	};
}
