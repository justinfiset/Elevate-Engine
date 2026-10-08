module;

#include <string>

export module Elevate.Core.Layers.Layer;

export namespace Elevate
{
	class Event;

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