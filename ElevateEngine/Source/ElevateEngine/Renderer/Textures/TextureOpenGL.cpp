#include <cstdint>
#include <memory>

#include <ElevateEngine/Platform/OpenGL/Textures/OpenGLTexture.h>
#include <ElevateEngine/Renderer/RendererBackend.h>

import Elevate.Foundations.Assert;
import Elevate.Renderer.Renderer;
import Elevate.Renderer.Texture;

namespace Elevate::RendererBackend
{
	void RegisterTextureFactory()
	{
		Texture::SetDataFactory([](const void* data, TextureMetadata& metadata) -> std::shared_ptr<Texture> {
			std::shared_ptr<Texture> texture;
			switch (Renderer::GetAPI())
			{
			case RendererAPI::GraphicAPI::None:
				texture = nullptr;
				break;
			case RendererAPI::GraphicAPI::OpenGL:
				texture = std::make_shared<OpenGLTexture>(data, metadata);
				break;
			default:
				Assert::That(false, "A supported RendererAPI needs to be supported!");
				return nullptr;
			}

			return TextureManager::RegisterTexture(texture);
		});
	}
}
