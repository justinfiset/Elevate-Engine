#include "eepch.h"

#include "GameContext.h"

namespace Elevate
{
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