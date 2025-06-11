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
		const void* data = nullptr;
		uint32_t size = 0;

		ComponentField() = default;
		ComponentField(const std::string& name, ComponentDataType type, const void* dataPtr)
			: name(name), type(type), data(dataPtr), size(GetDataTypeSize(type)) 
		{
			if (!dataPtr)
			{
				EE_CORE_ASSERT(false, "Cannot pass a null data ptr to a component field : {0}. Asserting.", name)
			}
		}
	};

	class ComponentLayout
	{
	public:
		ComponentLayout() = default;
		ComponentLayout(const std::string& name, std::initializer_list<ComponentField> fields) : m_name(name), m_fields(fields) { }

		// Regular and const operators to iterate easely trought fields
		std::vector<ComponentField>::iterator begin() { return m_fields.begin(); }
		std::vector<ComponentField>::iterator end() { return m_fields.end(); }
		std::vector<ComponentField>::const_iterator begin() const { return m_fields.begin(); }
		std::vector<ComponentField>::const_iterator end() const { return m_fields.end(); }

		const std::string& GetName() const { return m_name; }
	private:
		std::vector<ComponentField> m_fields;
		std::string m_name;
	};
}