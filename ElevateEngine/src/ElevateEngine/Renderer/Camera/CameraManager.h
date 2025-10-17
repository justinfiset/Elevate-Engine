#pragma once

#include <ElevateEngine/Core/Core.h>
#include <memory>

namespace Elevate
{
	class Camera;

	class CameraManager
	{
	public:
		static Camera* GetCurrent();

#ifdef EE_EDITOR_BUILD
		static Camera* GetEditor();
#endif
		static Camera* GetRuntime();

		static void SetCurrent(Camera* current);

		static void NotifyDestruction(Camera* camera);
	private:
		static Camera* s_currentCamera;
	};
}