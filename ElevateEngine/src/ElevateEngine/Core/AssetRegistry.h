#pragma once

#include <string>
#include <typeindex>
#include <filesystem>

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
		std::filesystem::path FilePath;
		std::type_index TypeIndex{ typeid(void) };

		bool isOnDisk = false;
		bool isLoaded = false;
		EEObjectPtr<Asset> Instance = nullptr;
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

		static const AssetMetaData* GetMetaFromExtension(const std::string& extension);
		static const AssetMetaData* GetMetaFromTypeName(const std::string& typeName);

		static void RegisterAssetType(AssetMetaData trait);
		static void RegisterAsset(const Asset* asset);

	protected:
		void _Init();
		void RefreshFromDisk();

		Guid ExtractGuidFromFile(std::filesystem::path path);

	private:
		static std::unordered_map<Guid, AssetEntry> s_indexedAssets;

		std::unordered_map<std::string, AssetMetaData> m_nameMeta;		// Key ex: Material
		std::unordered_map<std::string, AssetMetaData> m_extensionMeta;	// Key ex: .mat
	};
}
