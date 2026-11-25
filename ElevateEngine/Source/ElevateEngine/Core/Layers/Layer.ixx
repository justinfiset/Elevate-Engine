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

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}