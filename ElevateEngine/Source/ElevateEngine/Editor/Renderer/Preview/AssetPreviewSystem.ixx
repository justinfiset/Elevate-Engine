module;

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <cstdint>
#include <type_traits>
#include <typeinfo>

export module Elevate.Editor.Renderer.AssetPreviewSystem;

// Foundations
import Elevate.Foundations.Guid;
import Elevate.Foundations.Cleanups;
// Core
import Elevate.Core.Assets.Asset;
import Elevate.Core.Assets.AssetRegistry;
// Renderer
import Elevate.Renderer.Framebuffers;
// Editor
import Elevate.Editor.Renderer.AssetPreviewRenderer;

export namespace Elevate
{
	class AssetPreviewSystem : public IShutdownable
	{
	public:
		virtual ~AssetPreviewSystem() = default;

		template <typename AssetType, typename RendererType>
		void Register()
		{
			static_assert(std::is_base_of_v<AssetPreviewRenderer, RendererType>, "RendererType must derive from AssetPreviewRenderer");
			m_renderers[std::type_index(typeid(AssetType))] = std::make_unique<RendererType>();
		}

		void Remove(Guid assetGuid)
		{
			m_cache.erase(assetGuid);
		}

		void Shutdown() override
		{
			m_cache.clear();
			m_renderers.clear();
		}

		AssetPreviewRenderer* FindRenderer(std::type_index type) const
		{
			auto it = m_renderers.find(type);
			return it != m_renderers.end() ? it->second.get() : nullptr;
		}

		void Invalidate(const Guid& id)
		{
			if (auto it = m_cache.find(id); it != m_cache.end())
			{
				it->second.Revision = Entry::Invalid;
			}
		}

		void* GetPreview(const Guid& guid, std::uint32_t size = 256)
		{
			auto entry = AssetRegistry::GetEntry(guid);
			if (!entry || !m_renderers.contains(entry->TypeIndex))
			{
				return nullptr;
			}

			std::shared_ptr<Asset> asset = (entry->isLoaded && entry->Instance)
				? std::static_pointer_cast<Asset>(entry->Instance)
				: AssetRegistry::GetAsset<Asset>(entry->AssetGuid);

			return asset ? GetPreview(asset, size) : nullptr;
		}

		void* GetPreview(const std::shared_ptr<Asset>& asset, std::uint32_t size = 256)
		{
			if (!asset)
			{
				return nullptr;
			}

			AssetPreviewRenderer* renderer = FindRenderer(asset->GetTypeIndex());
			if (!renderer)
			{
				return nullptr;
			}

			auto [it, created] = m_cache.try_emplace(asset->GetGuid());
			Entry& entry = it->second;

			if (created)
			{
				entry.Target = renderer->CreatePreviewTarget(size);
			}

			if (entry.Revision != asset->GetRevision())
			{
				renderer->Render(asset, *entry.Target);
				entry.Revision = asset->GetRevision();
			}

			return entry.Target->GetColorAttachmentHandle();
		}

	private:
		struct Entry
		{
			static constexpr std::uint64_t Invalid = std::numeric_limits<std::uint64_t>::max();

			std::unique_ptr<Framebuffer> Target;
			std::uint64_t Revision = Invalid;
		};

		std::unordered_map<std::type_index, std::unique_ptr<AssetPreviewRenderer>> m_renderers;
		std::unordered_map<Guid, Entry> m_cache;
	};
}
