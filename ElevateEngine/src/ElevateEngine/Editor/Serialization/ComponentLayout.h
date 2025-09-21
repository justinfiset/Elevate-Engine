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
		std::vector<ComponentField> children;

		ComponentField() = default;
		ComponentField(const std::string& name, ComponentDataType type, const void* dataPtr) // TODO RETIRER
			: name(name), type(type), data(dataPtr), size(GetDataTypeSize(type)) { }

		ComponentField(const std::string& name, ComponentDataType type, size_t offset)
			: name(name), type(type), offset(offset), size(GetDataTypeSize(type)) { }

		ComponentField(
			const std::string& name,
			ComponentDataType type,
			size_t offset,
			const std::vector<ComponentField>& childrenFields
		)
			: name(name), type(type), offset(offset), size(GetDataTypeSize(type)), children(childrenFields)
		{}

		ComponentField(
			const std::string& name,
			ComponentDataType type,
			const void* data,
			const std::vector<ComponentField>& childrenFields
		)
			: name(name), type(type), offset(0), size(GetDataTypeSize(type)), data(data)
		{
			// copie récursive des enfants en ajustant leur data
			for (auto child : childrenFields) {
				ComponentField f = child;
				f.data = reinterpret_cast<const char*>(data) + f.offset;
				children.push_back(f);
			}
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
		size_t GetFieldCount() { return m_fields.size(); }

		const std::string& GetName() const { return m_name; }
	private:
		std::vector<ComponentField> m_fields;
		std::string m_name;
	};
}