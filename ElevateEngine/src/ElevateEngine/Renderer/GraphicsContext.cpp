#include "eepch.h"
#include "GraphicsContext.h"

namespace Elevate
{
	GraphicsContext* Elevate::GraphicsContext::s_context = nullptr;

	Elevate::GraphicsContext::GraphicsContext()
		: m_state(GraphicsContextState::UNINITIALIZED)
	{
		s_context = this;
	}

	Elevate::GraphicsContext::~GraphicsContext()
	{
		m_state = GraphicsContextState::TERMINATED;
	}
}