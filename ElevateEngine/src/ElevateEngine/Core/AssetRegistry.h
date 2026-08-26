#pragma once

#include <glm/vec4.hpp>
#include <string>
#include <typeindex>
#include <filesystem>

#include <ElevateEngine/Core/Guid.h>

namespace Elevate
{
	/// ////////////////////////////////////////////
	/// Asset Flags 
	/// ////////////////////////////////////////////
	
	enum class AssetFlags : uint8_t
	{
		None				= 0,
		CreateAssetMenu		= 1 << 0,
		HideInBrowser		= 1 << 1
	};

	inline AssetFlags operator|(AssetFlags a, AssetFlags b)
	{
		using T = std::underlying_type_t<AssetFlags>;
		return static_cast<AssetFlags>(static_cast<T>(a) | static_cast<T>(b));
	}

	inline AssetFlags& operator|=(AssetFlags& a, AssetFlags b)
	{
		a = a | b;
		return a;
	}

	inline bool operator&(AssetFlags a, AssetFlags b)
	{
		return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
	}

	/// ////////////////////////////////////////////
	/// Asset Meta Data
	/// ////////////////////////////////////////////

	struct AssetMetaData
	{
		std::string TypeName;
		std::string Extension;
		std::type_index TypeIndex{ typeid(void) };
		glm::vec4 AssetColor{ 0.5f, 0.5f, 0.5f, 1.0f };
		AssetFlags Flags = AssetFlags::None;
	};

	struct AssetEntry
	{
		Guid Guid;
		std::filesystem::path FilePath;
		std::type_index TypeIndex{ typeid(void) };
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

		static const AssetMetaData* GetMetaFromExtension(const std::string& extension);
		static const AssetMetaData* GetMetaFromTypeName(const std::string& typeName);

		static void RegisterAssetType(AssetMetaData trait);

	protected:
		void _Init();
		Guid ExtractGuidFromFile(std::filesystem::path path);

	private:
		static std::unordered_map<Guid, AssetEntry> s_indexedAssets;

		std::unordered_map<std::string, AssetMetaData> m_nameMeta;		// Key ex: Material
		std::unordered_map<std::string, AssetMetaData> m_extensionMeta;	// Key ex: .mat
	};
}
