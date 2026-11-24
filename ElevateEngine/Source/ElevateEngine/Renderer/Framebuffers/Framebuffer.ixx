module;


#include <ElevateEngine/Renderer/RendererBackend.h>
#include <memory>
#include <optional>
#include <vector>

#include "glm/glm.hpp"

export module Elevate.Renderer.Framebuffers:Framebuffer;

export import Elevate.Renderer.Texture;

export namespace Elevate
{
	struct FramebufferColorAttachment {
		std::shared_ptr<Texture> Tex = nullptr;
		uint8_t Index = 0;

		FramebufferColorAttachment() = default;
		FramebufferColorAttachment(std::shared_ptr<Texture> tex, uint32_t index = 0)
			: Tex(tex), Index(index)
		{
		}
	};

	struct FramebufferDepthAttachment {
		std::shared_ptr<Texture> Tex = nullptr;
		bool IsRenderbuffer = false;
	};

	class Framebuffer
	{
	public:
		using ViewportSetter = void (*)(int, int, int, int);
		using ClearColorSetter = void (*)(const glm::vec4&);
		using ClearFunction = void (*)();

		static void SetRendererFunctions(
			ViewportSetter viewportSetter,
			ClearColorSetter clearColorSetter,
			ClearFunction clearFunction);

		virtual ~Framebuffer() = default;

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void BlitFramebufferToScreen(uint32_t screenWidth, uint32_t screenHeight) const = 0;
		virtual void Rescale(uint32_t width, uint32_t height) = 0;
		virtual void Clear() const;

		void Use();
		void ClearAndUse();

		virtual void* GetColorAttachmentHandle(uint32_t index = 0) const;
		virtual void* GetDepthAttachmentHandle() const;

		virtual std::shared_ptr<Texture> GetColorTexture(uint32_t index = 0) const;
		virtual std::shared_ptr<Texture> GetDepthTexture() const;

		virtual uint32_t GetFrameBufferId() const = 0;
		inline void SetClearColor(glm::vec4 color) { m_clearColor = color; }

		virtual void BlitDepthTo(const Framebuffer& target) const = 0;
		virtual void BlitColorTo(const Framebuffer& target) const = 0;

		// Color + Depth + Stencil (most used case)
		static Framebuffer* Create(uint32_t width, uint32_t height, std::vector<TextureFormat> colorFormats, bool depthAsRenderbuffer = true, TextureType depthUsage = TextureType::Depth);
		// 0 Color and only 1 Depth texture
		static Framebuffer* CreateDepthOnly(uint32_t width = 2048, uint32_t height = 2048, TextureFormat depthFormat = TextureFormat::DEPTH, TextureType depthUsage = TextureType::Depth);
		// Pass custom config for the FrameBuffer
		using TextureFactory = Framebuffer* (*)(
			const std::vector<std::shared_ptr<Texture>>&,
			std::shared_ptr<Texture>,
			bool
		);

		static void SetTextureFactory(TextureFactory factory)
		{
			s_textureFactory = factory;
		}

		static Framebuffer* Create(
			const std::vector<std::shared_ptr<Texture>>& colorTextures,
			std::shared_ptr<Texture> depthTexture = nullptr,
			bool depthAsRenderbuffer = true
		)
		{
			RendererBackend::RegisterFramebufferFactory();
			return s_textureFactory ? s_textureFactory(colorTextures, depthTexture, depthAsRenderbuffer) : nullptr;
		}

	private:
		inline static TextureFactory s_textureFactory = nullptr;
		static ViewportSetter s_viewportSetter;
		static ClearColorSetter s_clearColorSetter;
		static ClearFunction s_clearFunction;

	protected:
		void UseViewport();

		static std::shared_ptr<Texture> CreateColorTexture(uint32_t width, uint32_t height, TextureFormat colorFormat = TextureFormat::RGBA);
		static std::shared_ptr<Texture> CreateDepthTexture(uint32_t width, uint32_t height, TextureFormat depthFormat = TextureFormat::DEPTH, TextureType usage = TextureType::Depth);

	protected:
		std::vector<FramebufferColorAttachment> m_colorAttachments;
		std::optional<FramebufferDepthAttachment> m_depthAttachment;
		glm::vec4 m_clearColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		uint32_t m_width = 0;
		uint32_t m_height = 0;
	};
}

