#pragma once
#ifdef EE_PLATFORM_WEB
#include "../Shared/GLFW/GlfwWindow.h"

namespace Elevate
{
	class WebWindow : public GlfwWindow 
	{
	public:
		WebWindow(const WindowProps& props)
		{
			Init(props);
		}

		void Init(const WindowProps& props) override
		{
			unsigned int width, height;
			GetCanvasSize(width, height);
			WindowProps webProps(props.Title, width, height, props.VSync);
			GlfwWindow::Init(webProps);

			emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_TRUE,
			[](int, const EmscriptenUiEvent*, void* ud) -> EM_BOOL {
				WebWindow* window = static_cast<WebWindow*>(ud);
				if (!window || !window->GetNativeWindow())
				{
					return EM_FALSE;
				}

				unsigned int width = 0;
				unsigned int height = 0;
				window->GetCanvasSize(width, height);
				window->SetWindowSize(width, height);
				glfwSetWindowSize(static_cast<GLFWwindow*>(window->GetNativeWindow()),
					static_cast<int>(width), static_cast<int>(height));
				return EM_TRUE;
			});
		}

		void GetCanvasSize(unsigned int& width, unsigned int& height)
		{
			double w, h;
			emscripten_get_element_css_size("#canvas", &w, &h);
			width = static_cast<unsigned int>(w);
			height = static_cast<unsigned int>(h);
		}
	};
}

#endif
