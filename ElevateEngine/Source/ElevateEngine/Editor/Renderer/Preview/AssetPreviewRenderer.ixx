module;


#include <cstdint>
#include <memory>

export module Elevate.Editor.Renderer.AssetPreviewRenderer;

// Core
import Elevate.Core.Assets.Asset;
// Renderer
import Elevate.Renderer.Framebuffers;

export namespace Elevate
{
	class AssetPreviewRenderer
	{
	public:
		virtual ~AssetPreviewRenderer() = default;

		virtual std::unique_ptr<Framebuffer> CreatePreviewTarget(std::uint32_t size = 256)
		{
			return std::unique_ptr<Framebuffer>(Framebuffer::Create(size, size, { TextureFormat::RGBA }, false));
		}

		virtual void Render(const std::shared_ptr<Asset>& asset, Framebuffer& framebuffer) = 0;
	};
}
