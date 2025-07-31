#pragma once

#include "glm/glm.hpp"
#include <ElevateEngine/Renderer/Texture/Texture.h>

namespace Elevate
{
	class FrameBuffer
	{
	public:
		~FrameBuffer() { }

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

		static FrameBuffer* Create(uint32_t width, uint32_t height);

	protected:
		FrameBuffer(TexturePtr tex) : m_texture(tex) { }
			
	protected:
		TexturePtr m_texture;
		glm::vec4 m_clearColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
}

