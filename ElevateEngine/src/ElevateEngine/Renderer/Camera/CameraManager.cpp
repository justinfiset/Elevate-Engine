#include "eepch.h"

#include "CameraManager.h"
#include "ElevateEngine/Core/Application.h"

#include <ElevateEngine/Core/Assert.h>

#ifdef EE_EDITOR_BUILD
#include "ElevateEngine/Editor/EditorLayer.h"
#include "ElevateEngine/Editor/Camera/EditorCamera.h"
#endif

namespace Elevate
{
	Camera* CameraManager::s_currentCamera = nullptr;

	Camera* CameraManager::GetCurrent()
	{
#ifdef EE_EDITOR_BUILD
		if (Application::GetGameState() == EditorMode)
		{
			return GetEditor();
		}
#endif

		Camera* runtime = GetRuntime();
		if (!runtime)
		{
			EE_CORE_ASSERT(false, "ERROR : There is no active camera! - CameraManager::GetCurrent()");
		}

		return runtime;
	}

#ifdef EE_EDITOR_BUILD
	Camera* CameraManager::GetEditor()
	{
		return Editor::EditorLayer::Get().GetCamera();
	}
#endif

	Camera* CameraManager::GetRuntime()
	{
		if (s_currentCamera)
		{
			return s_currentCamera;
		}
		return nullptr;
	}

	void CameraManager::SetCurrent(Camera* current)
	{
		s_currentCamera = current;
	}

	void CameraManager::NotifyDestruction(Camera* camera)
	{
		if (s_currentCamera && camera == s_currentCamera)
		{
			s_currentCamera = nullptr;
		}
	}
}