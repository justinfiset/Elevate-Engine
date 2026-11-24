#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <ElevateEngine/Platform/OpenGL/Framebuffer/OpenGLFramebuffer.h>
#include <ElevateEngine/Renderer/RendererBackend.h>

import Elevate.Foundations.Assert;
import Elevate.Renderer.Framebuffers;
import Elevate.Renderer.Renderer;
import Elevate.Renderer.RendererAPI;

namespace Elevate::RendererBackend
{
	void RegisterFramebufferFactory()
	{
		Framebuffer::SetTextureFactory([](
			const std::vector<std::shared_ptr<Texture>>& colorTextures,
			std::shared_ptr<Texture> depthTexture,
			bool depthAsRenderbuffer) -> Framebuffer* {
				switch (Renderer::GetAPI())
				{
				case RendererAPI::GraphicAPI::None:
					Assert::That(false, "GraphicsAPI::None is not supported!");
					return nullptr;

				case RendererAPI::GraphicAPI::OpenGL:
					return new OpenGLFramebuffer(colorTextures, depthTexture, depthAsRenderbuffer);
				}

				Assert::That(false, "Unknown GraphicsAPI!");
				return nullptr;
		});
	}
}
