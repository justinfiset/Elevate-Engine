#pragma once

#include <ElevateEngine/Core/TypeRegistry.h>

// Todo remove and add as a tag
#define EECATEGORY(name) \
    private: \
        inline static struct categoryRegistrar { \
            categoryRegistrar() { \
                generated_classEntry.Category = EECategory(name); \
            } \
        } generated_categoryRegistrar; \
        virtual EECategory GetCategory() const override { return generated_classEntry.Category; } \
    public:

namespace Elevate::Internal {
    template<typename T, typename = void>
    struct ScopeSelector {
        static std::vector<::Elevate::TypeField>& GetStack(T*) {
            return T::generated_structEntry.StructFieldStack;
        }
    };

    template<typename T>
    struct ScopeSelector<T, std::void_t<decltype(T::generated_classEntry)>> {
        static std::vector<::Elevate::TypeField>& GetStack(T*) {
            return T::generated_classEntry.ClassFieldStack;
        }
    };
}

// =======================================================
// BEGIN_OBJECT / DECLARE_BASE / PROPERTY / END_OBJECT
// =======================================================
#define BEGIN_OBJECT(T, ...) \
private: \
    using ThisType = T; \
public: \
    inline static struct T##ClassEntry { \
        T##ClassEntry() { \
            ::Elevate::TypeRegistry::AddClassToStack(#T); \
            ClassName = ::Elevate::TypeRegistry::GetCleanedName(#T); \
            HasBaseClass = false; \
            Options = { __VA_ARGS__ }; \
        } \
        std::vector<FieldOption> Options; \
        EECategory Category; \
        std::string ClassName; \
        std::vector<::Elevate::TypeField> ClassFieldStack; \
        bool HasBaseClass = false; \
    } generated_classEntry;

#define DECLARE_BASE(BaseType) \
private: \
    using Super = BaseType; \
    inline static struct BaseType##BaseClassDeclaration { \
        BaseType##BaseClassDeclaration() { \
            generated_classEntry.HasBaseClass = true; \
        } \
    } generated_baseDeclaration; \
public:

#define PROPERTY(param, ...) \
public: \
    inline static struct param##PropertyEntry { \
        param##PropertyEntry() { \
            using MemberT = decltype(ThisType::param); \
            auto& targetStack = ::Elevate::Internal::ScopeSelector<ThisType>::GetStack(static_cast<ThisType*>(nullptr)); \
            ::Elevate::TypeRegistry::AddPropertyDirect<ThisType, MemberT>( \
                &ThisType::param, \
                #param, \
                { __VA_ARGS__ }, \
                targetStack \
            ); \
        } \
    } generated_##param##PropertyEntry;

#define END_OBJECT() \
private: \
    inline static struct ClassEntryEnd { \
        ClassEntryEnd() { \
            ::Elevate::TypeRegistry::Register<ThisType>( \
                    generated_classEntry.ClassName, \
                    generated_classEntry.Options \
            ); \
            ::Elevate::TypeRegistry::PopClassStack(); \
        } \
    } generated_classEntryEnd; \
public: \
    inline virtual std::string GetName() const override { return generated_classEntry.ClassName; } \
    inline virtual ::Elevate::TypeLayout GetLayout() const override { \
        std::vector<::Elevate::TypeField> instanceFields; \
        if (generated_classEntry.HasBaseClass) { \
            auto parentFields = ParentFieldsHelper<ThisType>::Get(); \
            for (const ::Elevate::TypeField& field : parentFields) { \
                const void* fieldPtr = reinterpret_cast<const char*>(this) + field.offset; \
                instanceFields.push_back(::Elevate::TypeField(field, fieldPtr)); \
            } \
        } \
        for (const ::Elevate::TypeField& field : generated_classEntry.ClassFieldStack) { \
            const void* fieldPtr = reinterpret_cast<const char*>(this) + field.offset; \
            instanceFields.push_back(::Elevate::TypeField(field, fieldPtr)); \
        } \
        return ::Elevate::TypeLayout(generated_classEntry.ClassName, instanceFields); \
    } \
    virtual std::type_index GetTypeIndex() const override { return typeid(ThisType); }

// =======================================================
// BEGIN_COMPONENT / END_COMPONENT
// =======================================================
#define BEGIN_COMPONENT(T, ...) \
BEGIN_OBJECT(T, __VA_ARGS__) \
public: \
    virtual bool RemoveFromGameObject() override { \
        if (gameObject) { \
            gameObject->RemoveComponent<T>(); \
            return true; \
        } \
        return false; \
    }

#ifdef EE_EDITOR_BUILD
#define EDITOR_ONLY_COMPONENT_END_CODE(T) \
        virtual const void* GetEditorIconHandle() const override { \
            auto& entry = TypeRegistry::GetEntry<ThisType>(); \
            if (auto* trait = entry.GetTrait<::Elevate::EditorTypeTrait>()) { \
                if(!trait->editorIconPath.empty()) { \
                    return Texture::CreateFromFile(trait->editorIconPath)->GetNativeHandle(); \
                } \
            } \
            return nullptr; \
        }
#else
#define EDITOR_ONLY_COMPONENT_END_CODE(T)
#endif

#define END_COMPONENT() \
END_OBJECT() \
public: \
    inline static struct ComponentEntryEnd { \
        ComponentEntryEnd() { \
            ::Elevate::TypeRegistry::AddTrait<ThisType, ::Elevate::ComponentTypeTrait>(); \
            auto* trait = ::Elevate::TypeRegistry::GetEntry<ThisType>().GetTrait<::Elevate::ComponentTypeTrait>(); \
            if (trait) { \
                trait->category = generated_classEntry.Category; \
                trait->getter = [](std::weak_ptr<GameObject> go) -> Component* { \
                    if (std::shared_ptr<GameObject> obj = go.lock()) { \
                        return obj->GetComponent<ThisType>(); \
                    } \
                    return nullptr; \
                }; \
                trait->factory = [](std::weak_ptr<GameObject> go) -> Component* { \
                    if (std::shared_ptr<GameObject> obj = go.lock()) { \
                        return &obj->AddComponent<ThisType>(); \
                    } \
                    return nullptr; \
                }; \
                trait->destructor = [](std::weak_ptr<GameObject> go) -> void { \
                    if (std::shared_ptr<GameObject> obj = go.lock()) { \
                        obj->RemoveComponent<ThisType>(); \
                    } \
                }; \
            } \
        } \
    } generated_componentEntryEnd; \
    virtual Component* Clone() override { \
        return nullptr; \
    } \
    virtual void CopyFrom(Component* other) override { \
        if (auto o = dynamic_cast<ThisType*>(other)) { \
            if (generated_classEntry.HasBaseClass) { \
                auto parentFields = ParentFieldsHelper<ThisType>::Get(); \
                for (const ::Elevate::TypeField& field : parentFields) { \
                     /* field.CopyValue(o, this); */ \
                } \
            } \
            for (const ::Elevate::TypeField& field : generated_classEntry.ClassFieldStack) { \
                 /* field.CopyValue(o, this); */ \
            } \
        } \
        else { \
            EE_ERROR("Error: Tried setting a %s from a %s component in CopyFrom(Component*)", \
                this->GetName(), other ? other->GetName() : "null"); \
        } \
    } \
    virtual ::Elevate::GameObjectComponentFactory GetFactory() const override { \
        auto& entry = TypeRegistry::GetEntry<ThisType>(); \
        if (auto* trait = entry.GetTrait<::Elevate::ComponentTypeTrait>()) { \
            return trait->factory; \
        } \
        return nullptr; \
    } \
    virtual ::Elevate::GameObjectComponentDestructor GetDestructor() const override { \
        auto& entry = TypeRegistry::GetEntry<ThisType>(); \
        if (auto* trait = entry.GetTrait<::Elevate::ComponentTypeTrait>()) { \
            return trait->destructor; \
        } \
        return nullptr; \
    } \
    EDITOR_ONLY_COMPONENT_END_CODE(ThisType)

// =======================================================
// BEGIN_STRUCT / END_STRUCT
// =======================================================
#define BEGIN_STRUCT(T) \
private: \
    using ThisType = T; \
public: \
    inline static struct T##StructEntry { \
        T##StructEntry() { \
            ::Elevate::TypeRegistry::AddClassToStack(#T); \
            StructName = ::Elevate::TypeRegistry::GetCleanedName(#T); \
            StructTypeName = typeid(T).name(); \
        } \
        std::string StructName; \
        std::string StructTypeName; \
        std::vector<::Elevate::TypeField> StructFieldStack; \
    } generated_structEntry; \
public:

#define END_STRUCT() \
private: \
    inline static struct StructEntryEnd { \
        StructEntryEnd() { \
            ::Elevate::TypeRegistry::PopClassStack(); \
            ::Elevate::TypeRegistry::Register<ThisType>( \
                generated_structEntry.StructName, \
                std::vector<FieldOption>{} \
            ); \
            ::Elevate::TypeRegistry::GetReflectedTypes()[typeid(ThisType)] = generated_structEntry.StructFieldStack; \
        } \
    } generated_structEntryEnd;