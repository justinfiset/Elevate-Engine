#pragma once

class Application;

namespace Elevate
{
	class Time
	{
	public:
		static inline float GetCurrentTime() { return s_currentTime; }
		static inline float GetDeltaTime() { return s_deltaTime; }

	private:
		friend class Application;

		static inline float s_currentTime;
		static inline float s_deltaTime;
	};
}