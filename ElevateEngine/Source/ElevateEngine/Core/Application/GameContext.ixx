module;

#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>

export module Elevate.Core.Application.GameContext;

export namespace Elevate
{
	enum GameContextState
	{
		Initializing,
		EditorMode,
		Runtime,
		Paused,
		Stopped
	};

	const char* GetGameContextStateName(GameContextState state);

	using GameStateProvider = GameContextState (*)();

	void SetGameStateProvider(GameStateProvider provider);
	GameContextState GetGameState();
}