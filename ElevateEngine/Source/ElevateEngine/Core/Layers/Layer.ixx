module;

#include <string>

export module Elevate.Core.Layers.Layer;

import Elevate.Core.Events.Event;

export namespace Elevate
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach()
		{
			// No per-layer work by default.
		}

		virtual void OnDetach()
		{
			// No per-layer work by default.
		}

		virtual void OnUpdate()
		{
			// No per-layer work by default.
		}

		virtual void OnRender()
		{
			// No per-layer work by default.
		}

		virtual void OnImGuiRender()
		{
			// No per-layer work by default.
		}

		virtual void OnImGuiRenderBegin()
		{
			// No per-layer work by default.
		}

		virtual void OnImGuiRenderEnd()
		{
			// No per-layer work by default.
		}

		virtual void OnEvent(Event& event)
		{
			// No per-layer event handling by default.
		}

		virtual void Cleanup()
		{
			// No per-layer resources to release by default.
		}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}