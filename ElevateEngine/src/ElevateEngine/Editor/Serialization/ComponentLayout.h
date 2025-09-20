#pragma once
#include <string>
#include <vector>
#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/Assert.h>

namespace Elevate
{
	typedef EngineDataType ComponentDataType;

	struct ComponentField
	{
		std::string name;
		ComponentDataType type;
		const void* data = nullptr; // TODO RETIRER
		size_t offset = 0;
		uint32_t size = 0;

		ComponentField() = default;
		ComponentField(const std::string& name, ComponentDataType type, const void* dataPtr) // TODO RETIRER
			: name(name), type(type), data(dataPtr), size(GetDataTypeSize(type)) 
		{
			if (!dataPtr)
			{
				EE_CORE_ASSERT(false, "Cannot pass a null data ptr to a component field : {0}. Asserting.", name)
			}
		}

		ComponentField(const std::string& name, ComponentDataType type, size_t offset)
			: name(name), type(type), offset(offset), size(GetDataTypeSize(type))
		{}
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
		size_t GetFieldCount() { return m_fields.size(); }

		const std::string& GetName() const { return m_name; }
	private:
		std::vector<ComponentField> m_fields;
		std::string m_name;
	};
}