#include "eepch.h"
#include "ElevateEngine/Renderer/RendererAPI.h"

namespace Elevate
{
	// todo set the rendererAPI dinamicly and not staticly
	RendererAPI::GraphicAPI RendererAPI::s_ActiveAPI = RendererAPI::GraphicAPI::OpenGL;
}