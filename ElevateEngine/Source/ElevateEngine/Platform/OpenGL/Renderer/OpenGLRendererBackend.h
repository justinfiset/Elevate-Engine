#pragma once

namespace Elevate
{
	// OpenGL implementation of the engine-wide backend registration hook.
	// The concrete types stay behind this class so the engine never has to
	// import the platform layer.
	class OpenGLRendererBackend final
	{
	public:
		static void Register();
	};
}
