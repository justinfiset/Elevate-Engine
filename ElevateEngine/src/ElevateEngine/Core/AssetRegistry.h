#pragma once

#include <string>
#include <typeindex>
#include <filesystem>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Asset.h>
#include <ElevateEngine/Core/Guid.h>
#include <ElevateEngine/Core/AssetMetaData.h>

namespace Elevate
{
	/// ////////////////////////////////////////////
	/// Asset Entry
	/// ////////////////////////////////////////////

	struct AssetEntry
	{
		Guid Guid;
		std::string AssetName;
		std::filesystem::path FilePath;
		std::type_index TypeIndex{ typeid(void) };

		bool isOnDisk = false;
		bool isLoaded = false;
		const AssetMetaData* MetaData = nullptr;
		std::shared_ptr<Asset> Instance = nullptr;
	};

	/// ////////////////////////////////////////////
	/// Asset Registry
	/// ////////////////////////////////////////////

	class AssetRegistry
	{
	public:
		static AssetRegistry& Get()
		{
			static AssetRegistry instance;
			return instance;
		}
	
		static void Init();

		static std::vector<Guid> GetAssetsOfType(std::type_index typeIndex);
		static const AssetEntry* GetEntry(const Guid& guid);

		template<typename T>
		static std::shared_ptr<T> GetAsset(const Guid& guid)
		{
			static_assert(std::is_base_of_v<Asset, T>, "T must derive from Asset");

			if (!guid.IsValid())
			{
				EE_CORE_ERROR("Could not get asset for invaid guid.");
				return nullptr;
			}

			auto it = s_indexedAssets.find(guid);
			if (it == s_indexedAssets.end())
			{
				EE_CORE_ERROR("Asset with GUID {} is not indexed in AssetRegistry.", guid.ToString());
				return nullptr;
			}
			
			AssetEntry& entry = it->second;
			if (entry.isLoaded && entry.Instance)
			{
				return std::dynamic_pointer_cast<T>(entry.Instance);
			}
			
			if (entry.isOnDisk) // Lazy loading
			{
				std::shared_ptr<Asset> loadedAsset = LoadAssetFromDisk(entry);
				if (loadedAsset)
				{
					entry.Instance = loadedAsset;
					entry.isLoaded = true;
					return std::dynamic_pointer_cast<T>(loadedAsset);
				}
			}

			EE_CORE_ERROR("Failed to load asset {} from path: {}", guid.ToString(), entry.FilePath.string());
			return nullptr;
		}

		static const AssetMetaData* GetMetaFromExtension(const std::string& extension);
		static const AssetMetaData* GetMetaFromTypeName(const std::string& typeName);
		static const AssetMetaData* GetMetaFromTypeIndex(std::type_index typeIndex);

		static const std::unordered_map<std::string, AssetMetaData>& GetNameMetas();

		static void RegisterAssetType(AssetMetaData trait);

		static void RegisterAsset(const std::shared_ptr<Asset>& asset);
		static void RegisterAssetOnDisk(const std::shared_ptr<Asset>& asset, const std::filesystem::path& filePath);

	protected:
		void _Init();
		void RefreshFromDisk();

		Guid ExtractGuidFromFile(std::filesystem::path path);

	private:
		static std::shared_ptr<Asset> LoadAssetFromDisk(const AssetEntry& entry);

	private:
		static std::unordered_map<Guid, AssetEntry> s_indexedAssets;

		std::unordered_map<std::string, AssetMetaData> m_nameMeta;		// Key ex: Material
		std::unordered_map<std::string, AssetMetaData> m_extensionMeta;	// Key ex: .mat
		std::unordered_map<std::type_index, AssetMetaData> m_typeMeta;
	};
}
