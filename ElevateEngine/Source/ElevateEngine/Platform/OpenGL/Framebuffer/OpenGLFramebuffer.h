#pragma once

#include "ElevateEngine/Renderer/Framebuffers/Framebuffer.h"

namespace Elevate
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const std::vector<std::shared_ptr<Texture>>& colorTextures, std::shared_ptr<Texture> depthTexture, bool depthAsRenderbuffer);
		~OpenGLFramebuffer();

		void Bind() const override;
		void Unbind() const override;
		void BlitFramebufferToScreen(uint32_t screenWidth, uint32_t screenHeight) const override;
		void Rescale(uint32_t width, uint32_t height) override;

		virtual void BlitDepthTo(const Framebuffer& target) const override;

		inline uint32_t GetFrameBufferId() const override { return m_frameBufferId; }
	private:
		bool CheckCompleteness() const;
		const char* GetFramebufferStatusString(uint32_t status) const;
	private:
		// IDs
		uint32_t m_frameBufferId = 0;
		uint32_t m_renderBufferId = 0;
	};
}
