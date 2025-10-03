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
		GraphicsContext() : m_state(GraphicsContextState::UNINITIALIZED) 
		{
			s_context.reset(this);
		}
		
		~GraphicsContext()
		{
			m_state = GraphicsContextState::TERMINATED;
		}

		virtual void Init()
		{
			GraphicsContextState::ACTIVE;
		}

		virtual void SwapBuffers() = 0;
		
		inline bool CanUseContext()
		{
			return m_state == GraphicsContextState::ACTIVE;
		}

		static std::weak_ptr<GraphicsContext> Get()
		{
			return s_context;
		}
	private:
		GraphicsContextState m_state;

		static std::shared_ptr<GraphicsContext> s_context;
	};
}