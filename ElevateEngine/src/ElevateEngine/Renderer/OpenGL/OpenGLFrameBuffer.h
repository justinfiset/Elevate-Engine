#pragma once
#include "ElevateEngine/Renderer/FrameBuffer.h"

namespace Elevate
{
	class OpenGLFrameBuffer : public Framebuffer
	{
	public:
		OpenGLFrameBuffer(TexturePtr tex);

		void Bind() const override;
		void Unbind() const override;
		void BlitFramebufferToScreen(uint32_t screenWidth, uint32_t screenHeight) const override;
		void Rescale(uint32_t width, uint32_t height) override;

		inline uint32_t GetFrameBufferId() const override { return m_frameBufferId;  }
	private:
		bool CheckCompleteness() const;
		const char* GetFramebufferStatusString(uint32_t status) const;
	private:
		// IDs
		uint32_t m_frameBufferId;
		uint32_t m_renderBufferId;
		uint32_t m_textureId;
	};
}