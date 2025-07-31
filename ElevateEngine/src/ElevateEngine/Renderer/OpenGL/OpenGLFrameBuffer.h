#pragma once
#include "ElevateEngine/Renderer/FrameBuffer.h"

namespace Elevate
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(TexturePtr tex);

		void Bind() const override;
		void Unbind() const override;
		void Rescale(uint32_t width, uint32_t height) override;

		inline uint32_t GetFrameBufferId() const override { return m_FrameBufferId;  }
		inline uint32_t GetTextureId() const override { return m_TextureId; }
	private:
		bool CheckCompleteness() const;
		const char* GetFramebufferStatusString(uint32_t status) const;
	private:
		// IDs
		uint32_t m_FrameBufferId;
		uint32_t m_RenderBufferId;
		uint32_t m_TextureId;
	};
}