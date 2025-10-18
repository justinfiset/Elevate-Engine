#pragma once

#include "glm/glm.hpp"
#include <ElevateEngine/Renderer/Texture/Texture.h>


#include <optional>
#include <vector>

namespace Elevate
{
	struct FramebufferColorAttachment {
		TexturePtr Texture;
		uint8_t Index = 0;

		FramebufferColorAttachment(TexturePtr tex) : Texture(tex) { }
	};

	struct FramebufferDepthAttachment {
		void* NativeHandle = nullptr; // TODO CHANGER POUR METTRE UNE INTERFACE DE BINDABLE OU AUTRE
		bool IsRenderbuffer = false;
	};

	struct FramebufferStencilAttachment {

	};

	struct FramebufferMetadata {
		std::vector<FramebufferColorAttachment> ColorAttachments;
		std::optional<FramebufferDepthAttachment> DepthAttachment;
		std::optional<FramebufferDepthAttachment> Attachment;

		glm::vec4 ClearColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class Framebuffer
	{
	public:
		~Framebuffer() { }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Rescale(uint32_t width, uint32_t height) = 0;

		inline void* GetNativeTextureHandle() const {
			return m_texture->GetNativeHandle();
		}

		// TODO REMOVE
		virtual uint32_t GetFrameBufferId() const = 0;

		inline void SetClearColor(glm::vec4 color) { m_clearColor = color; }

		virtual void Clear() const;

		static Framebuffer* Create(uint32_t width = 1280, uint32_t height = 720);

	protected:
		Framebuffer(TexturePtr tex) : m_texture(tex) { }
			
	protected:
		TexturePtr m_texture; // TODO REMOVE
		glm::vec4 m_clearColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // TODO REMOVE
	};
}

