#pragma once

class Application;
namespace Elevate
{
	class Time
	{
	private:
		friend class Application;
	public:
		static inline float GetCurrentTime() { return currentTime_; }
		static inline float GetDeltaTime() { return deltaTime_; }
	private:
		static inline float currentTime_;
		static inline float deltaTime_;
	};
}