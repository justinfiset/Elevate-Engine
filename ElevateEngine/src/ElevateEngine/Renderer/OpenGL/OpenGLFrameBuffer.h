#pragma once
#include "ElevateEngine/Renderer/FrameBuffer.h"

// TODO try to remove by any way
#include <cstdint>
#include <glm/vec4.hpp>

namespace Elevate
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer() : OpenGLFrameBuffer(1280, 720) { }
		OpenGLFrameBuffer(uint32_t width, uint32_t height);

		void Bind() const override;
		void Unbind() const override;
		void Clear() const override;
		void Rescale(uint32_t width, uint32_t height) override;
		
		inline uint32_t GetFrameBufferId() const override { return m_FrameBufferId;  }
		inline uint32_t GetTextureId() const override { return m_TextureId; }
	private:
		// IDs
		uint32_t m_FrameBufferId;
		uint32_t m_RenderBufferId;
		uint32_t m_TextureId;

		// Size
		float m_Width;
		float m_Height;
	};
}