#pragma once

#include <memory>

namespace Elevate
{
	class Camera;

	class CameraManager
	{
	public:
		static Camera* GetCurrent();
		static void SetCurrent(Camera* current);
	private:
		static Camera* s_currentCamera;
	};
}