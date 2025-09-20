#pragma once

#include <typeindex>
#include <map>
#include <string>
#include <functional>
#include <entt/entt.hpp>
#include <stack>
#include <ElevateEngine/Core/Data.h>
#include <memory>

#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>

namespace Elevate {
    class Component;
}

namespace Elevate
{
    class ComponentRegistry {
    public:
        using ComponentFactory = std::function<Component* (entt::registry&, entt::entity)>;

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

        template<typename Class, typename FieldType>
        static void AddProperty(FieldType Class::* member, const std::string& name, EngineDataType type)
        {
            size_t offset = reinterpret_cast<size_t>(
                &(reinterpret_cast<Class const volatile*>(0)->*member)
                );

            EE_CORE_TRACE(" -- {}", GetCleanedName(name));

            CompilationClassFieldStack().push_back(ComponentField(
                GetCleanedName(name),
                type,
                offset
            ));
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
    using ThisComponentType = T; \
    inline static struct T##ClassEntry { \
        T##ClassEntry() { \
            FieldStartIndex = ::Elevate::ComponentRegistry::CompilationClassFieldStack().size(); \
            ::Elevate::ComponentRegistry::AddClassToStack(#T); \
            ClassName = ::Elevate::ComponentRegistry::GetCleanedName(#T); \
        } \
        size_t FieldStartIndex = 0; \
        std::string ClassName; \
        std::vector<Elevate::ComponentField> ClassFieldStack; \
    } generated_classEntry; \
    public: \
        inline virtual bool RemoveFromGameObject() override { \
            gameObject->RemoveComponent<T>(); \
            return true; \
        }

#define EXPOSE(param, type) \
    inline static struct param##PropertyEntry { \
        param##PropertyEntry() { \
            ::Elevate::ComponentRegistry::AddProperty<ThisComponentType>( \
                &ThisComponentType::param, \
                #param, \
                type \
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
            for (const auto& field : generated_classEntry.ClassFieldStack) { \
                const void* fieldPtr = reinterpret_cast<const char*>(this) + field.offset; \
                instanceFields.push_back(Elevate::ComponentField(field.name, field.type, fieldPtr)); \
            } \
            return Elevate::ComponentLayout(generated_classEntry.ClassName, instanceFields); \
        }
