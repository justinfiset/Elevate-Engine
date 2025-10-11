#include "eepch.h"

#include "CameraManager.h"
#include "ElevateEngine/Core/Application.h"

#include "ElevateEngine/Core/Core.h"

#ifdef EE_EDITOR_BUILD
#include "ElevateEngine/Editor/EditorLayer.h"
#endif

namespace Elevate
{
	Camera* CameraManager::s_currentCamera = nullptr;

	Camera* CameraManager::GetCurrent()
	{
		#ifdef EE_EDITOR_BUILD
		if (Application::GameState() == EditorMode)
		{
			return Editor::EditorLayer::Get().GetCamera();
		}
		#endif

		if (s_currentCamera)
		{
			return s_currentCamera;
		}
		else
		{
			EE_CORE_ASSERT(false, "ERROR : There is no active camera! - CameraManager::GetCurrent()");
		}

		return nullptr;
	}

	void CameraManager::SetCurrent(Camera* current)
	{
		s_currentCamera = current;
	}
}
