#pragma once

#include <typeindex>
#include <type_traits>

#include <entt/entt.hpp>

#include <stack>
#include <map>
#include <variant>

#include <memory>
#include <string>
#include <functional>

#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <ElevateEngine/Core/GameObject.h>

namespace Elevate
{
    template<typename T, typename = void>
    struct has_super : std::false_type {};
    template<typename T>
    struct has_super<T, std::void_t<typename T::Super>> : std::true_type {};

    template<typename T, bool = has_super<T>::value>
    struct ParentFieldsHelper {
        static std::vector<ComponentField> Get() { return {}; }
    };

    template<typename T>
    struct ParentFieldsHelper<T, true> {
        static std::vector<ComponentField> Get() {
            using Super = typename T::Super;
            if constexpr (!std::is_same_v<Super, void> && std::is_base_of_v<Component, Super>) {
                return Super::generated_classEntry.ClassFieldStack;
            }
            else {
                return {};
            }
        }
    };

    // Field Property ------------------------------------------------------
    struct FlattenTag {};
    #define Flatten FlattenTag{}

    struct DisplayNameTag { const char* value; };
    #define DisplayName(x) DisplayNameTag{x}

    struct TooltipTag { const char* text; };
    #define Tooltip(x) TooltipTag{x}

    struct ReadOnlyTag {};
    #define ReadOnly ReadOnlyTag{}

    struct ColorTag {};
    #define Color ColorTag{}

    using FieldOption = std::variant<
        FlattenTag, DisplayNameTag, TooltipTag, ReadOnlyTag, ColorTag
    >;

    struct FieldMeta {
        bool flatten = false;
        std::string displayName = "";
        std::string tooltip;
        bool readOnly = false;
        bool isColor = false;
    };
    // Field Property ^ ----------------------------------------------------

    class ComponentRegistry {
    public:
        using ComponentFactory = std::function<Component* (entt::registry&, entt::entity)>;

        template<typename T>
        static auto GetParentFieldsIfPossible(const T* obj) -> std::vector<ComponentField> {
            return ParentFieldsHelper<T>::Get(obj);
        }

        template<typename T>
        static void Register(const std::string& name) {
            GetFactories()[&typeid(T)] = [](entt::registry& registry, entt::entity entity) -> Component* {
                if (registry.all_of<T>(entity)) {
                    return static_cast<Component*>(&registry.get<T>(entity));
                }
                return nullptr;
            };

            GetTypeNames()[std::type_index(typeid(T))] = name;
        }

        static auto& GetFactories() {
            static std::unordered_map<const std::type_info*, ComponentFactory> factories;
            return factories;
        }

        static auto& GetTypeNames() {
            static std::unordered_map<std::type_index, std::string> names;
            return names;
        }

        static std::string GetName(const std::type_info& type) {
            auto& names = GetTypeNames();
            auto it = names.find(std::type_index(type));
            if (it != names.end()) {
                return it->second;
            }
            else {
                return type.name();
            }
        }

        static std::vector<std::string> s_classPaths;
        static std::vector<std::string>& CompilationClassStack() {
            static std::vector<std::string> stack;
            return stack;
        }

        struct IComponentPropertyEntry
        {
            virtual ~IComponentPropertyEntry() = default;

            virtual const std::string& GetName() const = 0;
            virtual EngineDataType GetType() const = 0;
            virtual size_t GetOffset() const = 0;

            virtual void* GetValuePtr(const void* instance) const = 0;
        };

        template<typename Class, typename FieldType>
        struct ComponentPropertyEntry : public IComponentPropertyEntry
        {
            using MemberPointer = FieldType Class::*;

            ComponentPropertyEntry(MemberPointer member, std::string name, EngineDataType type)
                : m_member(member), m_name(std::move(name)), m_type(type)
            {
                m_offset = reinterpret_cast<size_t>(
                    &(reinterpret_cast<Class const volatile*>(0)->*member)
                    );
            }

            const std::string& GetName() const override { return m_name; }
            EngineDataType GetType() const override { return m_type; }
            size_t GetOffset() const override { return m_offset; }

            FieldType& GetValue(Class& instance) const {
                return instance.*m_member;
            }

            void* GetValuePtr(const void* instance) const override {
                const Class* obj = static_cast<const Class*>(instance);
                auto ptr = &(obj->*m_member);
                return const_cast<void*>(reinterpret_cast<const void*>(ptr));
            }

        private:
            MemberPointer m_member;
            std::string m_name;
            EngineDataType m_type;
            size_t m_offset;
        };

        static inline std::vector<ComponentField>& CompilationClassFieldStack()
        {
            static std::vector<ComponentField> stack;
            return stack;
        }

        template<typename T>
        struct EngineDataTypeTrait
        {
            static constexpr EngineDataType value = EngineDataType::Custom;
        };

        template<> struct EngineDataTypeTrait<float> { static constexpr EngineDataType value = EngineDataType::Float; };
        template<> struct EngineDataTypeTrait<int> { static constexpr EngineDataType value = EngineDataType::Int; };
        template<> struct EngineDataTypeTrait<bool> { static constexpr EngineDataType value = EngineDataType::Bool; };
        template<> struct EngineDataTypeTrait<glm::vec2> { static constexpr EngineDataType value = EngineDataType::Float2; };
        template<> struct EngineDataTypeTrait<glm::vec3> { static constexpr EngineDataType value = EngineDataType::Float3; };
        template<> struct EngineDataTypeTrait<glm::vec4> { static constexpr EngineDataType value = EngineDataType::Float4; };
        template<typename T>
        static constexpr EngineDataType DeduceEngineDataType() { return EngineDataTypeTrait<std::decay_t<T>>::value; }

        static std::string GetCleanedName(std::string rawName)
        {
            std::string cleanedName = "";

            for (int i = 0; i < rawName.length(); i++)
            {
                char c = rawName.at(i); // Current char
                if (i == 0)
                {
                    if ((c == 'm' || c == 's' || c == 'g') && rawName.at(i + 1) == '_') // If we have a notation of Hungarian style : m_myVariable
                    {
                        i = 1;
                        continue;
                    }
                    else
                    {
                        cleanedName += std::toupper(c);
                    }
                }
                else if (std::isupper(c) && std::islower(rawName.at(i + 1)))
                {
                    cleanedName += ' ';
                    cleanedName += c;
                }
                else if (std::isalpha(c) && rawName.at(i - 1) == '_')
                {
                    cleanedName += std::toupper(c);
                }
                else if (c == '_')
                {
                    cleanedName += ' ';
                }
                else
                {
                    cleanedName += c;
                }
            }

            return cleanedName;
        }

        static void AddClassToStack(std::string newClass)
        {
            EE_CORE_INFO(GetCleanedName(newClass));
            CompilationClassStack().push_back(newClass);
        }

        static std::map<std::string, std::vector<ComponentField>>& GetCustomComponentFields()
        {
            static std::map<std::string, std::vector<ComponentField>> m_customComponentFields;
            return m_customComponentFields;
        }

        template<typename Class, typename FieldType>
        static void AddProperty(FieldType Class::* member, const std::string& name, std::initializer_list<FieldOption> options)
        {
            constexpr EngineDataType type = DeduceEngineDataType<FieldType>();
            FieldMeta meta;
            for (auto&& opt : options) {
                if (std::holds_alternative<FlattenTag>(opt)) {
                    meta.flatten = true;
                }
                else if (std::holds_alternative<DisplayNameTag>(opt)) {
                    meta.displayName = std::get<DisplayNameTag>(opt).value;
                }
                else if (std::holds_alternative<TooltipTag>(opt)) {
                    meta.tooltip = std::get<TooltipTag>(opt).text;
                }
                else if (std::holds_alternative<ReadOnlyTag>(opt)) {
                    meta.readOnly = true;
                }
                else if (std::holds_alternative<ColorTag>(opt)) {
                    meta.isColor = true;
                }
            }

            EE_CORE_TRACE(" --> Exposed field : {0} flatten={1}  displayName={2}", GetCleanedName(name), meta.flatten, meta.displayName);

            auto& customFields = GetCustomComponentFields();
            std::string typeName = typeid(FieldType).name();
            size_t offset = reinterpret_cast<size_t>(&(reinterpret_cast<Class const volatile*>(0)->*member));

            ComponentField field;
            if (customFields.find(typeName) != customFields.end())
            {
                field = ComponentField(GetCleanedName(name), EngineDataType::Custom, offset, meta.displayName, customFields[typeName]);
                field.flatten = meta.flatten;
            }
            else
            {
                field = ComponentField(GetCleanedName(name), type, offset, meta.displayName);
            }
            field.isColor = meta.isColor;
            field.tooltip = meta.tooltip;
            field.readOnly = meta.readOnly;
            CompilationClassFieldStack().push_back(field);
        }

        static void PopClassStack()
        {
            try
            {
                std::string fullName;
                for (int i = 0; i < CompilationClassStack().size(); i++)
                {
                    fullName.append(CompilationClassStack()[i]);
                    if (i != CompilationClassStack().size() - 1)
                    {
                        fullName.append("/");
                    }
                }
                CompilationClassStack().pop_back();
                s_classPaths.push_back(fullName);
            }
            catch (const std::exception&)
            {
                EE_CORE_ERROR("More BEGIN_COMPONENT() then END_COMPONENT, did you forgot to add a END_COMPONENT() to the end of your class?");
            }
        }
    };

    enum class EntryType
    {
        Component,
        Struct
    };

    static std::stack<EntryType>& GetEntryTypes()
    {
        static std::stack<EntryType> s_entryTypes;
        return s_entryTypes;
    }
}

// TODO REMOVE THIS OBSELETE VERSION
#define REGISTER_COMPONENT(T) \
    namespace { \
        struct T##Registrar { \
            T##Registrar() { \
                Elevate::ComponentRegistry::Register<T>(#T); \
            } \
        } T##_registrar; \
    }

// =======================================================
// BEGIN_COMPONENT / EXPOSE / END_COMPONENT
// =======================================================

#define BEGIN_COMPONENT(T) \
private: \
using ThisType = T; \
public: \
inline static struct T##ClassEntry { \
    T##ClassEntry() { \
        Elevate::ComponentRegistry::Register<T>(#T); \
        FieldStartIndex = ::Elevate::ComponentRegistry::CompilationClassFieldStack().size(); \
        ::Elevate::ComponentRegistry::AddClassToStack(#T); \
        ClassName = ::Elevate::ComponentRegistry::GetCleanedName(#T); \
        HasBaseClass = false; \
    } \
    size_t FieldStartIndex = 0; \
    std::string ClassName; \
    std::vector<Elevate::ComponentField> ClassFieldStack; \
    bool HasBaseClass = false; \
} generated_classEntry; \
public: \
    virtual bool RemoveFromGameObject() override { \
        if (gameObject) { \
            gameObject->RemoveComponent<T>(); \
            return true; \
        } \
        return false; \
    }

#define EXPOSE(param, ...) \
public: \
inline static struct param##PropertyEntry { \
    param##PropertyEntry() { \
        using MemberT = decltype(ThisType::param); \
        ::Elevate::ComponentRegistry::AddProperty<ThisType, MemberT>( \
            &ThisType::param, \
            #param, \
            { __VA_ARGS__ } \
        ); \
    } \
} generated_##param##PropertyEntry;

#define END_COMPONENT() \
private: \
    inline static struct ClassEntryEnd { \
        ClassEntryEnd() { \
            ::Elevate::ComponentRegistry::PopClassStack(); \
            auto& global = ::Elevate::ComponentRegistry::CompilationClassFieldStack(); \
            size_t start = generated_classEntry.FieldStartIndex; \
            for (size_t i = start; i < global.size(); ++i) { \
                generated_classEntry.ClassFieldStack.push_back(global[i]); \
            } \
            if (start < global.size()) { \
                global.erase(global.begin() + start, global.end()); \
            } \
        } \
    } generated_classEntryEnd; \
public: \
    inline virtual std::string GetName() const override { return generated_classEntry.ClassName; } \
    inline virtual Elevate::ComponentLayout GetLayout() const override { \
        std::vector<Elevate::ComponentField> instanceFields; \
        if (generated_classEntry.HasBaseClass) { \
            auto parentFields = ParentFieldsHelper<ThisType>::Get(); \
            for (const Elevate::ComponentField& field : parentFields) { \
                const void* fieldPtr = reinterpret_cast<const char*>(this) + field.offset; \
                instanceFields.push_back(Elevate::ComponentField( \
                    field, fieldPtr \
                )); \
            } \
        } \
        for (const Elevate::ComponentField& field : generated_classEntry.ClassFieldStack) { \
            const void* fieldPtr = reinterpret_cast<const char*>(this) + field.offset; \
            instanceFields.push_back(Elevate::ComponentField(field, fieldPtr)); \
        } \
        return Elevate::ComponentLayout(generated_classEntry.ClassName, instanceFields); \
    } \

#define DECLARE_BASE(BaseType) \
using Super = BaseType; \
inline static struct BaseType##BaseClassDeclaration { \
    BaseType##BaseClassDeclaration() { \
        generated_classEntry.HasBaseClass = true; \
    } \
} generated_baseDeclaration;

#define BEGIN_STRUCT(T) \
    private: \
    using ThisType = T; \
    inline static struct T##StructEntry { \
        T##StructEntry() { \
            ::Elevate::ComponentRegistry::AddClassToStack(#T); \
            FieldStartIndex = ::Elevate::ComponentRegistry::CompilationClassFieldStack().size(); \
            StructName = ::Elevate::ComponentRegistry::GetCleanedName(#T); \
            StructTypeName = typeid(T).name(); \
        } \
        size_t FieldStartIndex = 0; \
        std::string StructName; \
        std::string StructTypeName; \
        std::vector<Elevate::ComponentField> StructFieldStack; \
    } generated_structEntry; \
    public:

#define END_STRUCT() \
    private: \
    inline static struct StructEntryEnd { \
        StructEntryEnd() { \
            auto& global = ::Elevate::ComponentRegistry::CompilationClassFieldStack(); \
            size_t start = generated_structEntry.FieldStartIndex; \
            for (size_t i = start; i < global.size(); ++i) { \
                generated_structEntry.StructFieldStack.push_back(global[i]); \
            } \
            if (start < global.size()) { \
                global.erase(global.begin() + start, global.end()); \
            } \
            ::Elevate::ComponentRegistry::GetCustomComponentFields()[generated_structEntry.StructTypeName] = generated_structEntry.StructFieldStack; \
        } \
    } generated_structEntryEnd;
