module;

#include <ElevateEngine/Core/Scene/SceneEnTTOperators.h>

module Elevate.Renderer.RendererAPI;

namespace Elevate
{
	// todo set the rendererAPI dinamicly and not staticly
	RendererAPI::GraphicAPI RendererAPI::s_ActiveAPI = RendererAPI::GraphicAPI::OpenGL;
}