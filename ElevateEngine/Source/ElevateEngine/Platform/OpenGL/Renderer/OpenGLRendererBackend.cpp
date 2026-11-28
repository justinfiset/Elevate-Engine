// Every textual include must be processed before the first module import.
// GCC does not reprocess standard headers after an import, and GLM types must
// be established textually before any module BMI exports them.
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <new>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

#include <ElevateEngine/Renderer/RendererBackend.h>
#include "OpenGLRendererBackend.h"

// These headers include GLM textually before their own imports, so they must
// come before every import-bearing header below.
#include <ElevateEngine/Platform/OpenGL/Renderer/OpenGLRendererAPI.h>
#include <ElevateEngine/Platform/OpenGL/Shaders/OpenGLShader.h>

// Import-bearing OpenGL headers last: GLM and the standard library are already
// fully defined above, so their imports cannot introduce conflicting types.
#include <ElevateEngine/Platform/OpenGL/Buffers/OpenGLBuffer.h>
#include <ElevateEngine/Platform/OpenGL/Buffers/OpenGLVertexArray.h>
#include <ElevateEngine/Platform/OpenGL/Framebuffer/OpenGLFramebuffer.h>
#include <ElevateEngine/Platform/OpenGL/Textures/OpenGLTexture.h>

import Elevate.Renderer.Renderer;
import Elevate.Renderer.Buffers;
import Elevate.Renderer.VertexArray;
import Elevate.Renderer.Framebuffers;
import Elevate.Renderer.Shaders;
import Elevate.Renderer.Texture;

namespace Elevate
{
	void OpenGLRendererBackend::Register()
	{
		static bool registered = false;
		if (registered)
		{
			return;
		}
		registered = true;

		Renderer::SetAPIFactory([]() -> std::unique_ptr<RendererAPI> {
			return std::make_unique<OpenGLRendererAPI>();
		});

		VertexBuffer::SetFactory([](const std::byte* vertices, uint32_t size) -> VertexBuffer* {
			return new OpenGLVertexBuffer(vertices, size);
		});

		IndexBuffer::SetFactory([](const std::byte* indices, uint32_t count) -> IndexBuffer* {
			return new OpenGLIndexBuffer(indices, count);
		});

		VertexArray::SetFactory([]() -> VertexArray* {
			return new OpenGLVertexArray();
		});

		Framebuffer::SetTextureFactory([](
			const std::vector<std::shared_ptr<Texture>>& colorTextures,
			std::shared_ptr<Texture> depthTexture,
			bool depthAsRenderbuffer) -> Framebuffer* {
				return new OpenGLFramebuffer(colorTextures, std::move(depthTexture), depthAsRenderbuffer);
		});

		Shader::SetNativeFactories(
			[]() -> std::shared_ptr<Shader> {
				return std::make_shared<OpenGLShader>(
					std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetVertexShader()),
					std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetFragmentShader())
				);
			},
			[]() -> std::shared_ptr<Shader> {
				return std::make_shared<OpenGLShader>(
					std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetVertexShader()),
					std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetErrorShader())
				);
			}
		);

		Shader::SetFactory([](const std::string& vertexSource, const std::string& fragmentSource) -> std::shared_ptr<Shader> {
			return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
		});

		Texture::SetDataFactory([](const std::byte* data, TextureMetadata& metadata) -> std::shared_ptr<Texture> {
			return TextureManager::RegisterTexture(std::make_shared<OpenGLTexture>(data, metadata));
		});
	}
}

namespace Elevate::RendererBackend
{
	void Register()
	{
		OpenGLRendererBackend::Register();
	}
}
