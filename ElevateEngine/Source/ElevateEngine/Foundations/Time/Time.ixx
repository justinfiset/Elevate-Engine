export module Elevate.Foundations.Time;

export namespace Elevate
{
	class Time
	{
	public:
		static float GetCurrentTime() { return s_currentTime; }
		static float GetDeltaTime() { return s_deltaTime; }

		/// <summary>
		/// Update the current clock parameters. Should only be called by the main loop.
		/// </summary>
		/// <param name="currentTime">The current time.</param>
		/// <param name="deltaTime">The delta time for the current frame.</param>
		static void Update(float currentTime, float deltaTime)
		{
			s_deltaTime = deltaTime;
			s_currentTime = currentTime;
		}

	private:
		static inline float s_currentTime;
		static inline float s_deltaTime;
	};
}