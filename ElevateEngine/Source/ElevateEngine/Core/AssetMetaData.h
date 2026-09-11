#pragma once

#include <string>
#include <cstdint>
#include <typeindex>
#include <glm/vec4.hpp>

namespace Elevate
{
	/// ////////////////////////////////////////////
	/// Asset Flags 
	/// ////////////////////////////////////////////

	enum class AssetFlags : uint8_t
	{
		None = 0,
		CreateAssetMenu = 1 << 0,
		HideInBrowser = 1 << 1
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
}
