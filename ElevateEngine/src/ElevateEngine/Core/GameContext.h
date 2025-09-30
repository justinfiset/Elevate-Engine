#pragma once

namespace Elevate
{
	enum GameContextState
	{
		Initializing,
		EditorMode,
		Runtime,
		Paused,
		Stopped
	};

	class GameContext
	{
	protected:
		inline GameContextState State() const { return m_state; }

		inline void SetState(GameContextState newState)
		{
			if (m_state != newState)
			{
				GameContextState oldState = m_state;
				m_state = newState;
				OnStateChange(oldState, newState);
			}
		}
		virtual void OnStateChange(GameContextState oldState, GameContextState newState) { }
	private:
		// TODO DEFINE DEPENDING ON THE CURRENT BUILD TYPE
		GameContextState m_state = EditorMode;
	};
}