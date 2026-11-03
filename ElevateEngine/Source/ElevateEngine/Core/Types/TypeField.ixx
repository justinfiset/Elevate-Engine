module;

#include <cstdint>
#include <functional>
#include <string>
#include <typeindex>
#include <vector>

export module Elevate.Core.Types.TypeField;

export import Elevate.Foundations.Data;

export namespace Elevate
{
	struct TypeField
	{
		std::string name;
		std::string displayName;
		std::string tooltip;
		EngineDataType type;
		const void* data = nullptr;
		size_t offset = 0;
		uint32_t size = 0;
		bool flatten = false;
		bool readOnly = false;
		bool isColor = false;
		const void* targetTypeKey = nullptr;
		std::vector<TypeField> children;

		EngineDataType elementType = EngineDataType::Custom;
		std::vector<TypeField> elementChildren;

		// Array
		std::function<size_t(const void*)> GetArraySize;
		std::function<const void* (const void* instance, size_t index)> GetElementAddress;

		std::function<void(void* instance, size_t newSize)> ResizeArray;

		const void* (*GetPointee)(const void*) = nullptr;
		std::function<std::type_index(const void*)> GetPointeeTypeIndex;

		TypeField() = default;

		TypeField(const TypeField&) = default;
		TypeField& operator=(const TypeField&) = default;

		TypeField(TypeField&&) noexcept = default;
		TypeField& operator=(TypeField&&) noexcept = default;

		TypeField(
			const std::string& name,
			EngineDataType type,
			size_t offset,
			const std::string& displayName = ""
		)
			: name(name), type(type), offset(offset), displayName(displayName)
		{
			if (type == EngineDataType::Custom || type == EngineDataType::Array)
			{
				size = 0;
			}
			else
			{
				size = GetDataTypeSize(type);
			}
		}

		TypeField(
			const std::string& name,
			EngineDataType type,
			size_t offset,
			const std::string& displayName,
			const std::vector<TypeField>& childrenFields
		)
			: name(name), type(type), offset(offset), displayName(displayName), children(childrenFields)
		{
			if (type == EngineDataType::Custom || type == EngineDataType::Array)
			{
				size = 0;
			}
			else
			{
				size = GetDataTypeSize(type);
			}
		}

		TypeField(const std::string& name, EngineDataType type, const void* dataPtr)
			: name(name), type(type), data(const_cast<void*>(dataPtr)) {
		}

		TypeField(const TypeField& original, const void* dataPtr)
			: name(original.name),
			displayName(original.displayName),
			tooltip(original.tooltip),
			type(original.type),
			data(dataPtr),
			offset(original.offset),
			size(original.size),
			flatten(original.flatten),
			readOnly(original.readOnly),
			isColor(original.isColor),
			targetTypeKey(original.targetTypeKey),
			elementType(original.elementType),
			elementChildren(original.elementChildren),
			GetArraySize(original.GetArraySize),
			GetElementAddress(original.GetElementAddress),
			ResizeArray(original.ResizeArray),
			GetPointee(original.GetPointee)
		{
			for (const auto& child : original.children)
			{
				const void* childDataPtr = reinterpret_cast<const char*>(dataPtr) + child.offset;

				children.emplace_back(child, childDataPtr);
			}
		}

		std::type_index GetTargetType() const;

		void UseInstance(const void* instancePtr);

		inline const std::string& GetDisplayName() const
		{
			if (displayName.empty())
			{
				return name;
			}
			return displayName;
		}

		inline bool HasChildrens()
		{
			return !children.empty() || type == EngineDataType::Array;
		}
	};
}