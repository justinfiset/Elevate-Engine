#include "WebWindow.h"

#ifdef EE_PLATFORM_WEB
#include <ElevateEngine/Core/Log.h>
#include <emscripten/html5.h>

namespace Elevate
{
    WebWindow::WebWindow(const WindowProps& props)
    {
        Init(props);
    }

    void WebWindow::GetCanvasSize(unsigned int& width, unsigned int& height)
    {
        double w, h;
        emscripten_get_element_css_size("#canvas", &w, &h);
        width = static_cast<unsigned int>(w);
        height = static_cast<unsigned int>(h);
    }

    void WebWindow::Init(const WindowProps& props)
    {
        // Create the window with the size of the canvas element in the HTML page
        unsigned int width, height;
        GetCanvasSize(width, height);
        WindowProps webProps(props.Title, width, height, props.VSync);
        EE_CORE_INFO("Initializing WebWindow with canvas size: {}x{}", width, height);
        GlfwWindow::Init(webProps);

        emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_TRUE, 
        [](int et, const EmscriptenUiEvent* uiEvent, void* ud) -> EM_BOOL {
            WebWindow* window = static_cast<WebWindow*>(ud);
            unsigned int width = 0;
            unsigned int height = 0;
            window->GetCanvasSize(width, height);
            window->SetWindowSize(width, height);

            GLFWwindow* nativeWin = static_cast<GLFWwindow*>(window->GetNativeWindow());
            glfwSetWindowSize(nativeWin, width, height);

            return EM_TRUE;
        });
    }
}   

#endif