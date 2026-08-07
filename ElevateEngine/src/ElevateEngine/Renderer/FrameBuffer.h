#pragma once

#include "glm/glm.hpp"
#include <ElevateEngine/Renderer/Texture/Texture.h>


#include <optional>
#include <vector>

namespace Elevate
{
	struct FramebufferColorAttachment {
		TexturePtr Texture = nullptr;
		uint8_t Index = 0;

		FramebufferColorAttachment() = default;
		FramebufferColorAttachment(TexturePtr tex, uint32_t index = 0)
			: Texture(tex), Index(index) { }
	};

	struct FramebufferDepthAttachment {
		TexturePtr Texture = nullptr;
		bool IsRenderbuffer = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void BlitFramebufferToScreen(uint32_t screenWidth, uint32_t screenHeight) const = 0;
		virtual void Rescale(uint32_t width, uint32_t height) = 0;
		virtual void Clear() const;

		virtual void* GetColorAttachmentHandle(uint32_t index = 0) const;
		virtual void* GetDepthAttachmentHandle() const;

		virtual TexturePtr GetDepthTexture() const;

		virtual uint32_t GetFrameBufferId() const = 0;
		inline void SetClearColor(glm::vec4 color) { m_clearColor = color; }

		// Color + Depth + Stencil (most used case)
		static Framebuffer* Create(uint32_t width = 1280, uint32_t height = 720, uint32_t colorAttachmentCount = 1, bool depthAsRenderbuffer = true);
		// 0 Color and only 1 Depth texture
		static Framebuffer* CreateDepthOnly(uint32_t width = 2048, uint32_t height = 2048);
		// Pass custom config for the FrameBuffer
		static Framebuffer* Create(
			const std::vector<TexturePtr>& colorTextures,
			TexturePtr depthTexture = nullptr,
			bool depthAsRenderbuffer = true
		);

	private:
		static TexturePtr CreateColorTexture(uint32_t width, uint32_t height);
		static TexturePtr CreateDepthTexture(uint32_t width, uint32_t height);

	protected:
		std::vector<FramebufferColorAttachment> m_colorAttachments;
		std::optional<FramebufferDepthAttachment> m_depthAttachment;
		glm::vec4 m_clearColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		uint32_t m_width = 0;
		uint32_t m_height = 0;
	};
}

