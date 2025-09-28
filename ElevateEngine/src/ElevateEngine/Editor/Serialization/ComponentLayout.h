#pragma once
#include <string>
#include <vector>
#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/Assert.h>

namespace Elevate
{
	class Component;

	typedef EngineDataType ComponentDataType;

	struct ComponentField
	{
		std::string name;
		std::string displayName;
		std::string tooltip;
		ComponentDataType type;
		const void* data = nullptr;
		size_t offset = 0;
		uint32_t size = 0;
		bool flatten = false;
		bool readOnly = false;
		bool isColor = false;
		std::vector<ComponentField> children;

		ComponentField() = default;

		ComponentField(
			const std::string& name,
			ComponentDataType type, 
			size_t offset,
			const std::string& displayName = ""
		)
			: name(name), type(type), offset(offset), displayName(displayName) 
		{
			if (type == ComponentDataType::Custom)
			{
				size = 0;
			}
			else
			{
				size = GetDataTypeSize(type);
			}
		}

		ComponentField(
			const std::string& name,
			ComponentDataType type,
			size_t offset,
			const std::string& displayName,
			const std::vector<ComponentField>& childrenFields
		)
			: name(name), type(type), offset(offset), displayName(displayName), children(childrenFields)
		{
			if (type == ComponentDataType::Custom)
			{
				size = 0;
			}
			else
			{
				size = GetDataTypeSize(type);
			}
		}

		ComponentField(const std::string& name, ComponentDataType type, const void* dataPtr)
			: name(name), type(type), data(dataPtr) { }

		ComponentField(const ComponentField& original, const void* dataPtr)
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

		void CopyValue(Component* src, Component* clone) const
		{
			const void* srcData = reinterpret_cast<const char*>(src) + offset;
			void* destData = reinterpret_cast<char*>(clone) + offset;
			
			if (size > 0)
			{
				std::memcpy(destData, srcData, size);
			}
			else if (!children.empty())
			{
				for (auto& child : children)
				{
					child.CopyValue(src, clone);
				}
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

	class ComponentLayout
	{
	public:
		ComponentLayout() = default;
		ComponentLayout(const std::string& name, std::initializer_list<ComponentField> fields) : m_name(name), m_fields(fields) { }
		ComponentLayout(const std::string& name, std::vector<ComponentField>& fields) : m_name(name), m_fields(fields) { }

		// Regular and const operators to iterate easely trought fields
		std::vector<ComponentField>::iterator begin() { return m_fields.begin(); }
		std::vector<ComponentField>::iterator end() { return m_fields.end(); }
		std::vector<ComponentField>::const_iterator begin() const { return m_fields.begin(); }
		std::vector<ComponentField>::const_iterator end() const { return m_fields.end(); }
		size_t GetFieldCount() const { return m_fields.size(); }
		const std::vector<ComponentField>& GetFields() const { return m_fields; }


		const std::string& GetName() const { return m_name; }
	private:
		std::vector<ComponentField> m_fields;
		std::string m_name;
	};
}