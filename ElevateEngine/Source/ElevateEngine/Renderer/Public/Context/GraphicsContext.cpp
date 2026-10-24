module Elevate.Renderer.GraphicsContext;

namespace Elevate
{
	GraphicsContext *Elevate::GraphicsContext::s_context = nullptr;

	Elevate::GraphicsContext::GraphicsContext()
		: m_state(GraphicsContextState::Uninitialized)
	{
		s_context = this;
	}

	Elevate::GraphicsContext::~GraphicsContext()
	{
		m_state = GraphicsContextState::Terminated;
	}
}
