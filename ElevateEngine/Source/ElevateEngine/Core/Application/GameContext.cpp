module;

module Elevate.Core.Application.GameContext;

namespace Elevate
{
	namespace
	{
		GameStateProvider s_gameStateProvider = nullptr;
	}

	void SetGameStateProvider(GameStateProvider provider)
	{
		s_gameStateProvider = provider;
	}

	GameContextState GetGameState()
	{
		return s_gameStateProvider ? s_gameStateProvider() : GameContextState::Initializing;
	}

	const char* GetGameContextStateName(GameContextState state)
	{
		switch (state)
		{
			case Initializing: return "Initializing";
			case EditorMode: return "EditorMode";
			case Runtime: return "Runtime";
			case Paused: return "Paused";
			case Stopped: return "Stopped";
			default: return "Unknown";
		}
	}
}