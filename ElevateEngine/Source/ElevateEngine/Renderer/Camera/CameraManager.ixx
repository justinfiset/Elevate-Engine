module;

#include <memory>

export module Elevate.Renderer.Camera:CameraManager;

import :Fwd;

export namespace Elevate
{
	class CameraManager
	{
	public:
		static Camera* GetCurrent();

#ifdef EE_EDITOR_BUILD
		using EditorCameraProvider = Camera* (*)();

		static Camera* GetEditor();
		static void SetEditorCameraProvider(EditorCameraProvider provider);
#endif

		static Camera* GetRuntime();

		static void SetCurrent(Camera* current);

		static void NotifyDestruction(Camera* camera);

	private:
#ifdef EE_EDITOR_BUILD
		static EditorCameraProvider s_editorCameraProvider;
#endif
		static Camera* s_currentCamera;
	};
}