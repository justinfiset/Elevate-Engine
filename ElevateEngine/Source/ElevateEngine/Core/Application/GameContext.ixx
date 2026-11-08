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
}