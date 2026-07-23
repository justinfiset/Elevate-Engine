#pragma once
#include <string>
#include <cstring>
#include <vector>

#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/TypeField.h>

namespace Elevate
{
    struct PropertyField;
    using PropertySet = std::vector<PropertyField>;
    struct PropertyContainer;

    class TypeLayout
    {
    public:
        TypeLayout() = default;
        TypeLayout(const std::string& name, std::initializer_list<TypeField> fields) : m_name(name), m_fields(fields) {}
        TypeLayout(const std::string& name, std::vector<TypeField>& fields) : m_name(name), m_fields(fields) {}

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