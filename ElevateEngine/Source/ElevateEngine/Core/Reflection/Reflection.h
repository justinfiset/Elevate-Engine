#pragma once

#include <vector>
#include <string>
#include <typeindex>

import Elevate.Foundations.CoreLogger;
import Elevate.Core.Objects.Category;
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Types.TypeField;
import Elevate.Core.Types.TypeLayout;
import Elevate.Core.Types.TypeRegistry;
import Elevate.Core.Reflection.ReflectionTags;
import Elevate.Serialization.PropertyField;

#ifdef EE_EDITOR_BUILD
import Elevate.Renderer.Texture;
import Elevate.Editor.Types.EditorTypeTrait;
import Elevate.Editor.Icons;
#endif

#define EE_REQUIRE_SEMICOLON static_assert(true, "")

// todo : do not use macro in the future
#ifdef EE_EDITOR_BUILD
#define EE_EditorTag(x) x
#else
#define EE_EditorTag(x) ::Elevate::EmptyTag{}
#endif

#define EE_CreateAssetMenu EE_EditorTag(CreateAssetMenuTag{})
#define EE_AssetColor(r, g, b) EE_EditorTag((AssetColorTag{r, g, b}))
#define EE_Asset(name, extension, ...) EE_EditorTag(AssetTag{BuildAssetMetadata<ThisType>(name, extension, ##__VA_ARGS__)})

// =======================================================
// BEGIN_ENUM / ENUM_VALUE / END_ENUM
// =======================================================

#define BEGIN_ENUM(EnumName) \
    inline static struct EnumName##EnumEntry { \
        using EnumType = EnumName; \
        EnumName##EnumEntry() { \
            ::Elevate::TypeRegistry::RegisterEnum<EnumType>( \
                #EnumName, \
                {

#define ENUM_VALUE(name) \
                    { #name, static_cast<int64_t>(EnumType::name) },

#define END_ENUM(EnumName) \
                } \
            ); \
        } \
    } generated_##EnumName##EnumEntryInstance;

#include "ReflectionInternal.h"

// Todo remove and add as a tag
#define EECATEGORY(name) \
private: \
    inline static struct categoryRegistrar { \
        categoryRegistrar() { \
            ThisType::generated_classEntry.Category = ::Elevate::EECategory(name); \
        } \
    } generated_categoryRegistrar; \
public: \
    virtual ::Elevate::EECategory GetCategory() const override { return ThisType::generated_classEntry.Category; } \
    EE_REQUIRE_SEMICOLON

// =======================================================
// DECLARE_AUTO_OBJECT_NAME / DECLARE_AUTO_OBJECT_LAYOUT
// =======================================================

#define DECLARE_AUTO_OBJECT_NAME() \
public: \
    inline virtual std::string GetName() const override { return generated_classEntry.ClassName; } \
    EE_REQUIRE_SEMICOLON

#define DECLARE_AUTO_OBJECT_LAYOUT() \
public: \
    virtual ::Elevate::TypeLayout GetLayout() const override { \
        std::vector<::Elevate::TypeField> allFields; \
        allFields.push_back(::Elevate::TypeField( \
            "m_guid", \
            ::Elevate::EngineDataType::GUID, \
            ::Elevate::EEObject::GetGuidOffset() \
        )); \
        if constexpr (requires { typename ThisType::Super; }) { \
            auto& registryMap = ::Elevate::TypeRegistry::GetReflectedTypes(); \
            auto it = registryMap.find(typeid(typename ThisType::Super)); \
            if (it != registryMap.end()) { \
                for (const auto& field : it->second) { \
                    allFields.push_back(field); \
                } \
            } \
        } \
        for (const ::Elevate::TypeField& field : generated_classEntry.ClassFieldStack) { \
            allFields.push_back(field); \
        } \
        std::vector<::Elevate::TypeField> instanceFields; \
        for (const ::Elevate::TypeField& field : allFields) { \
            ::Elevate::TypeField instField = field; \
            instField.data = reinterpret_cast<const char*>(this) + field.offset; \
            instanceFields.push_back(instField); \
        } \
        return ::Elevate::TypeLayout(this, generated_classEntry.ClassName, instanceFields); \
    } \
    EE_REQUIRE_SEMICOLON

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
    } generated_classEntry; \
    EE_REQUIRE_SEMICOLON

#define DECLARE_BASE(BaseType) \
private: \
    using Super = BaseType; \
    inline static struct BaseType##BaseClassDeclaration { \
        BaseType##BaseClassDeclaration() { \
            generated_classEntry.HasBaseClass = true; \
        } \
    } generated_baseDeclaration; \
public: \
    EE_REQUIRE_SEMICOLON

#define PROPERTY(param, ...) \
private: \
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
    } generated_##param##PropertyEntry; \
public: \
    EE_REQUIRE_SEMICOLON

#define END_OBJECT_CUSTOM() \
private: \
    inline static struct ClassEntryEnd { \
        ClassEntryEnd() { \
            ::Elevate::TypeRegistry::Register<ThisType>( \
                    generated_classEntry.ClassName, \
                    generated_classEntry.Options \
            ); \
            ::Elevate::TypeRegistry::PopClassStack(); \
            ::Elevate::TypeRegistry::GetReflectedTypes()[typeid(ThisType)] = generated_classEntry.ClassFieldStack; \
        } \
    } generated_classEntryEnd; \
public: \
    virtual std::type_index GetTypeIndex() const override { return typeid(ThisType); } \
    EE_REQUIRE_SEMICOLON

#define END_OBJECT() \
    END_OBJECT_CUSTOM(); \
    DECLARE_AUTO_OBJECT_LAYOUT(); \
    DECLARE_AUTO_OBJECT_NAME();

// =======================================================
// BEGIN_COMPONENT / END_COMPONENT
// =======================================================
#define BEGIN_COMPONENT(T, ...) \
BEGIN_OBJECT(T, __VA_ARGS__); \
public: \
    virtual bool RemoveFromGameObject() override { \
        if (gameObject) { \
            gameObject->RemoveComponent<T>(); \
            return true; \
        } \
        return false; \
    } \
    EE_REQUIRE_SEMICOLON

#ifdef EE_EDITOR_BUILD
#define EDITOR_ONLY_COMPONENT_END_CODE(T) \
        virtual ::std::shared_ptr<::Elevate::Texture> GetEditorIcon() const override { \
            auto& entry = TypeRegistry::GetEntry<ThisType>(); \
            if (auto* trait = entry.GetTrait<::Elevate::EditorTypeTrait>()) { \
                if(!trait->editorIconPath.empty()) { \
                    return Texture::CreateFromFile(trait->editorIconPath); \
                } \
            } \
            return nullptr; \
        } \
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
END_OBJECT(); \
public: \
    inline static struct ComponentEntryEnd { \
        ComponentEntryEnd() { \
            ::Elevate::TypeRegistry::AddTrait<ThisType, ::Elevate::ComponentTypeTrait>(); \
            auto* trait = ::Elevate::TypeRegistry::GetEntry<ThisType>().GetTrait<::Elevate::ComponentTypeTrait>(); \
            if (trait) { \
                trait->category = generated_classEntry.Category; \
                trait->getter = [](void* go) -> void* \
                { \
                    auto* obj = static_cast<GameObject*>(go); \
                    return obj ? obj->GetComponent<ThisType>() : nullptr; \
                }; \
                trait->const_getter = [](const void* go) -> const void* \
                { \
                    auto* obj = static_cast<const GameObject*>(go); \
                    return obj ? obj->GetComponent<ThisType>() : nullptr; \
                }; \
                trait->factory = [](void* go) -> void* { \
                    auto* obj = static_cast<GameObject*>(go); \
                    if (obj) { \
                        return &obj->AddComponent<ThisType>(); \
                    } \
                    return nullptr; \
                }; \
                trait->destructor = [](void* go) -> void { \
                    auto* obj = static_cast<GameObject*>(go); \
                    if (obj) { \
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
            CoreLogger::Error("Error: Tried setting a %s from a %s component in CopyFrom(Component*)", \
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
    void SetFromProperties(const ::Elevate::PropertySet& props) override { \
        GetLayout().ApplyState(props); \
    } \
    EDITOR_ONLY_COMPONENT_END_CODE(ThisType) \
    EE_REQUIRE_SEMICOLON

// =======================================================
// BEGIN_STRUCT / END_STRUCT
// =======================================================
#define BEGIN_STRUCT(T) \
public: \
    using ThisType = T; \
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
    EE_REQUIRE_SEMICOLON

#define END_STRUCT() \
private: \
    inline static struct StructEntryEnd { \
        StructEntryEnd() { \
            ::Elevate::TypeRegistry::PopClassStack(); \
            ::Elevate::TypeRegistry::Register<ThisType>( \
                generated_structEntry.StructName, \
                std::vector<::Elevate::FieldOption>{} \
            ); \
            ::Elevate::TypeRegistry::GetReflectedTypes()[typeid(ThisType)] = generated_structEntry.StructFieldStack; \
        } \
    } generated_structEntryEnd; \
public: \
    inline ::Elevate::TypeLayout GetLayout() const { \
        std::vector<::Elevate::TypeField> instanceFields; \
        for (const ::Elevate::TypeField& field : generated_structEntry.StructFieldStack) { \
            ::Elevate::TypeField instField = field; \
            instField.data = reinterpret_cast<const char*>(this) + field.offset; \
            instanceFields.push_back(instField); \
        } \
        return ::Elevate::TypeLayout(this, generated_structEntry.StructName, instanceFields); \
    } \
    std::type_index GetTypeIndex() const { return typeid(ThisType); } \
    inline ::Elevate::PropertySet GetProperties() const { \
        return GetLayout().CaptureState(); \
    } \
    inline void SetFromProperties(const ::Elevate::PropertySet& props) { \
        GetLayout().ApplyState(props); \
    } \
    EE_REQUIRE_SEMICOLON
