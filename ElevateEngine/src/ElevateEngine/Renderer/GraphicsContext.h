#pragma once

namespace Elevate {
	enum class GraphicsContextState
	{
		UNINITIALIZED,
		ACTIVE,
		TERMINATED
	};

	class GraphicsContext
	{
	public:
		GraphicsContext();
		~GraphicsContext();

		virtual void Init()
		{
			GraphicsContextState::ACTIVE;
		}

		virtual void SwapBuffers() = 0;

		static GraphicsContext* Get()
		{
			return s_context;
		}

		static bool Valid()
		{
			return s_context != nullptr;
		}

		static bool CanUseContext()
		{
			return Valid() && s_context->m_state == GraphicsContextState::ACTIVE;
		}
	private:
		GraphicsContextState m_state;

		static GraphicsContext* s_context;
	};
}