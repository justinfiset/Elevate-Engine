#include "eepch.h"
#include "RendererAPIr.h"

namespace Elevate
{
	// todo set the rendererAPI dinamicly and not staticly
	RendererAPI::GraphicAPI RendererAPI::s_ActiveAPI = RendererAPI::GraphicAPI::OpenGL;
}