#include "eepch.h"
#include "CubemapTexture.h"
#include "ElevateEngine/Renderer/OpenGL/Texture/OpenGLCubemapTexture.h"
#include "ElevateEngine/Renderer/Renderer.h"

Elevate::CubemapTexturePtr Elevate::CubemapTexture::Create(std::string path)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::GraphicAPI::None: return nullptr; break; // TODO implement
	case RendererAPI::GraphicAPI::OpenGL: return std::make_shared<OpenGLCubemapTexture>(path);
	}

	EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
}
