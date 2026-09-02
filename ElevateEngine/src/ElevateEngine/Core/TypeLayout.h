#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <initializer_list>

#include <ElevateEngine/Core/TypeField.h>

namespace Elevate
{
    struct PropertyField;
    using PropertySet = std::vector<PropertyField>;

    class TypeLayout
    {
    public:
        TypeLayout() = default;
        TypeLayout(const void* target, const std::string& name, const std::initializer_list<TypeField> fields);
        TypeLayout(const void* target, const std::string& name, const std::vector<TypeField>& fields);

        // Regular and const operators to iterate easely trought fields
        std::vector<TypeField>::iterator begin() { return m_fields.begin(); }
        std::vector<TypeField>::iterator end() { return m_fields.end(); }
        std::vector<TypeField>::const_iterator begin() const { return m_fields.begin(); }
        std::vector<TypeField>::const_iterator end() const { return m_fields.end(); }

        size_t GetFieldCount() const { return m_fields.size(); }
        const std::vector<TypeField>& GetFields() const { return m_fields; }

        const std::string& GetName() const { return m_name; }

        static void BindFieldRecursively(TypeField& field, const void* parentAddress);
        void BindInstance(const void* instancePtr);

        PropertySet CaptureState() const;
        void ApplyState(const PropertySet& props);

    private:
        const void* m_objectInstance = nullptr;
        std::vector<TypeField> m_fields;
        std::string m_name;
    };
}