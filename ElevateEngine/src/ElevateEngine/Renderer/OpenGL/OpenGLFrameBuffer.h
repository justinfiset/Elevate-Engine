#pragma once
#include "ElevateEngine/Renderer/FrameBuffer.h"

// TODO try to remove by any way
#include <cstdint>

namespace Elevate
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer() : OpenGLFrameBuffer(1280, 720) { }
		OpenGLFrameBuffer(float width, float height);

		void Bind() const;
		void Unbind() const;
		void Rescale(float width, float height) const;
		uint32_t GetTextureID() const { return m_TextureId; }
	private:
		uint32_t m_FrameBufferId;
		uint32_t m_RenderBufferId;
		uint32_t m_TextureId;
	};
}