module;

module Elevate.Renderer.Camera;

// Foundations
import Elevate.Foundations.Assert;
// Core
import Elevate.Core.Application;

namespace Elevate
{
#ifdef EE_EDITOR_BUILD
	CameraManager::EditorCameraProvider CameraManager::s_editorCameraProvider = nullptr;
#endif
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
			Assert::That(false, "ERROR : There is no active camera! - CameraManager::GetCurrent()");
		}

		return runtime;
	}

#ifdef EE_EDITOR_BUILD
	Camera* CameraManager::GetEditor()
	{
		return s_editorCameraProvider ? s_editorCameraProvider() : nullptr;
	}

	void CameraManager::SetEditorCameraProvider(EditorCameraProvider provider)
	{
		s_editorCameraProvider = provider;
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