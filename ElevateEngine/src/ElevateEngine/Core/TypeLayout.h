#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <functional>
#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/Assert.h>

namespace Elevate
{
	struct PropertyField;
	using PropertySet = std::vector<PropertyField>;
	struct PropertyContainer;

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
		std::vector<TypeField> children;

		std::function<size_t(const void*)> GetArraySize;
		std::function<const void* (const void* instance, size_t index)> GetElementAddress;

		TypeField() = default;

		TypeField(
			const std::string& name,
			EngineDataType type, 
			size_t offset,
			const std::string& displayName = ""
		)
			: name(name), type(type), offset(offset), displayName(displayName) 
		{
			if (type == EngineDataType::Custom)
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
			if (type == EngineDataType::Custom)
			{
				size = 0;
			}
			else
			{
				size = GetDataTypeSize(type);
			}
		}

		TypeField(const std::string& name, EngineDataType type, const void* dataPtr)
			: name(name), type(type), data(dataPtr) { }

		TypeField(const TypeField& original, const void* dataPtr)
			: name(original.name), type(original.type), offset(original.offset), size(original.size), 
			displayName(original.displayName), flatten(original.flatten), readOnly(original.readOnly),
			isColor(original.isColor), data(dataPtr) 
		{
			for (const auto& child : original.children)
			{
				const void* childDataPtr = reinterpret_cast<const char*>(dataPtr) + child.offset;
				children.emplace_back(child, childDataPtr);
			}
		}

		inline const std::string& GetDisplayName() const
		{
			if (displayName.empty())
				return name;
			return displayName;
		}

		inline bool HasChildrens() { return !children.empty(); }
	};

	class TypeLayout
	{
	public:
		TypeLayout() = default;
		TypeLayout(const std::string& name, std::initializer_list<TypeField> fields) : m_name(name), m_fields(fields) { }
		TypeLayout(const std::string& name, std::vector<TypeField>& fields) : m_name(name), m_fields(fields) { }

		// Regular and const operators to iterate easely trought fields
		std::vector<TypeField>::iterator begin() { return m_fields.begin(); }
		std::vector<TypeField>::iterator end() { return m_fields.end(); }
		std::vector<TypeField>::const_iterator begin() const { return m_fields.begin(); }
		std::vector<TypeField>::const_iterator end() const { return m_fields.end(); }
		size_t GetFieldCount() const { return m_fields.size(); }
		const std::vector<TypeField>& GetFields() const { return m_fields; }

		const std::string& GetName() const { return m_name; }

		PropertySet CaptureState();

	private:
		std::vector<TypeField> m_fields;
		std::string m_name;
	};
}