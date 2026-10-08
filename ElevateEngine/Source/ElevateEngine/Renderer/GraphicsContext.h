#pragma once

namespace Elevate {
	enum class GraphicsContextState
	{
		Uninitialized,
		Active,
		Terminated
	};

	class GraphicsContext
	{
	public:
		GraphicsContext();
		~GraphicsContext();

		virtual void Init()
		{
			s_context->m_state = GraphicsContextState::Active;
		}

		virtual void SwapBuffers() = 0;

		static const GraphicsContext* Get() const
		{
			return s_context;
		}

		static bool IsValid() const
		{
			return s_context != nullptr;
		}

		static bool CanUseContext()
		{
			return IsValid() && s_context->m_state == GraphicsContextState::Active;
		}
	private:
		GraphicsContextState m_state;

		static GraphicsContext* s_context;
	};
}