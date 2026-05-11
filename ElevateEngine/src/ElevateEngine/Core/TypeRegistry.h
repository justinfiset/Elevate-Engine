#pragma once

#include <initializer_list>
#include <map>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <variant>

#include <entt/entt.hpp>
#include <glm/fwd.hpp>

#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>

#ifdef EE_RELEASE
	#undef EE_REGISTRY_LOG
#endif

namespace Elevate
{
	class GameObject;

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

	// Field / Component Property Tag ------------------------------------------------------
	struct HideInInspectorTag {};
	#define HideInInspector HideInInspectorTag{}

	struct FlattenTag {};
	#define Flatten FlattenTag{}

	struct DisplayNameTag { const char* value; };
	#define DisplayName(x) DisplayNameTag{x}

	struct TooltipTag { const char* text; };
	#define Tooltip(x) TooltipTag{x}

	struct ReadOnlyTag {};
	#define ReadOnly ReadOnlyTag{}

	struct ColorTag {};
	#define ColorPicker ColorTag{}

	struct EditorIconTag 
	{
		std::string Path;
	};
	#define EditorIcon(path) EditorIconTag{path}

	using FieldOption = std::variant<
		HideInInspectorTag, EditorIconTag,
		FlattenTag, DisplayNameTag, TooltipTag, ReadOnlyTag, ColorTag
	>;

	struct FieldMeta {
		bool flatten = false;
		std::string displayName = "";
		std::string tooltip;
		bool readOnly = false;
		bool isColor = false;
		std::string IconPath;
	};

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

	struct ITypeTrait
	{
		virtual ~ITypeTrait() = default;
	};

	struct EditorTypeTrait : public ITypeTrait
	{
		bool visible = true;
		std::string editorIconPath = "";

		EditorTypeTrait() = default;

		EditorTypeTrait(const std::vector<FieldOption>& options)
		{
			for (auto& option : options)
			{
				if (std::holds_alternative<HideInInspectorTag>(option)) {
					visible = false;
				}
				else if (std::holds_alternative<EditorIconTag>(option)) {
					editorIconPath = std::get<EditorIconTag>(option).Path;
				}
			}
		}
	};

	struct ComponentTypeTrait : public ITypeTrait
	{
		EECategory category;
		GameObjectComponentGetter getter; // method to get the type of component from a gameobject
		GameObjectComponentFactory factory; // factory to create / add to a gameObject
		GameObjectComponentDestructor destructor; // component destructor / remove from a gameObject
	};

	class TypeRegistry {
	public:
		template<typename T>
		static auto GetParentFieldsIfPossible(const T* obj) -> std::vector<ComponentField> {
			return ParentFieldsHelper<T>::Get(obj);
		}

		struct Entry
		{
			std::string name;
			std::type_index type{ typeid(void) };
			std::map<std::type_index, std::shared_ptr<ITypeTrait>> traits;

			Entry() = default;
			Entry(const std::string& name, std::type_index& type)
				: name(name), type(type) { }

			/**
			 * Function to get a specific variation of ITypeTraits added to the current object type.
			 * 
			 * @return the Trait typed asked. 
			 */
			template<typename T>
			T* GetTrait() const
			{
				auto it = traits.find(typeid(T));
				return (it != traits.end()) ? static_cast<T*>(it->second.get()) : nullptr;
			}
		};

		template<typename T>
		static void Register(const std::string& name, const std::vector<FieldOption>& options);
		
		template<typename T, typename Trait, typename... Args>
		static void AddTrait(Args&&... args)
		{
			auto& entry = GetEntry<T>();
			entry.traits[typeid(Trait)] = std::make_shared<Trait>(std::forward<Args>(args)...);
		}

		static std::unordered_map<std::type_index, Entry>& GetEntries()
		{
			static std::unordered_map<std::type_index, Entry> entries;
			return entries;
		}

		template<typename T>
		static Entry& GetEntry()
		{
			return GetEntries()[typeid(T)];
		}

		static std::string GetName(const std::type_info& type);

		static std::vector<std::string>& ClassPaths() {
			static std::vector<std::string> paths;
			return paths;
		}

		static std::vector<std::string>& CompilationClassStack() {
			static std::vector<std::string> stack;
			return stack;
		}

		static inline std::vector<ComponentField>& CompilationClassFieldStack()
		{
			static std::vector<ComponentField> stack;
			return stack;
		}

		template<typename T>
		static constexpr EngineDataType DeduceEngineDataType() 
		{
			return EngineDataTypeTrait<std::decay_t<T>>::value;
		}

		// Simple method to convert a variable or class name to a display name, ex: m_myProperty -> My Property
		static std::string GetCleanedName(std::string rawName);

		static void AddClassToStack(std::string newClass);
		static void PopClassStack();

		static std::map<std::type_index, std::vector<ComponentField>>& GetCustomComponentFields()
		{
			static std::map<std::type_index, std::vector<ComponentField>> m_customComponentFields;
			return m_customComponentFields;
		}

		template<typename Class, typename FieldType>
		static void AddProperty(FieldType Class::* member, const std::string& name, std::initializer_list<FieldOption> options);
	};
}

#define EECATEGORY(name) \
	private: \
		inline static struct categoryRegistrar { \
			categoryRegistrar() { \
				generated_classEntry.Category = EECategory(name); \
			} \
		} generated_categoryRegistrar; \
		virtual EECategory GetCategory() const override { return generated_classEntry.Category; } \
	public:

#include "TypeRegistry.inl"

// =======================================================
// BEGIN_OBJECT / DECLARE_BASE / PROPERTY / END_OBJECT
// =======================================================
#define BEGIN_OBJECT(T, ...) \
private: \
	using ThisType = T; \
public: \
	inline static struct T##ClassEntry { \
		T##ClassEntry() { \
			FieldStartIndex = ::Elevate::TypeRegistry::CompilationClassFieldStack().size(); \
			::Elevate::TypeRegistry::AddClassToStack(#T); \
			ClassName = ::Elevate::TypeRegistry::GetCleanedName(#T); \
			HasBaseClass = false; \
			Options = { __VA_ARGS__ }; \
		} \
		std::vector<FieldOption> Options; \
		EECategory Category; \
		size_t FieldStartIndex = 0; \
		std::string ClassName; \
		std::vector<Elevate::ComponentField> ClassFieldStack; \
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
			::Elevate::TypeRegistry::AddProperty<ThisType, MemberT>( \
				&ThisType::param, \
				#param, \
				{ __VA_ARGS__ } \
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
			auto& global = ::Elevate::TypeRegistry::CompilationClassFieldStack(); \
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
		ThisType* clone = new ThisType(); \
		for (auto& field : TypeRegistry::GetCustomComponentFields()[typeid(ThisType)]) { \
			field.CopyValue(this, clone); \
		} \
		return clone; \
	} \
	virtual void CopyFrom(Component* other) override { \
		if (auto o = dynamic_cast<ThisType*>(other)) { \
			if (generated_classEntry.HasBaseClass) { \
				auto parentFields = ParentFieldsHelper<ThisType>::Get(); \
				for (const Elevate::ComponentField& field : parentFields) { \
					field.CopyValue(o, this); \
				} \
			} \
			for (const Elevate::ComponentField& field : generated_classEntry.ClassFieldStack) { \
				field.CopyValue(o, this); \
			} \
		} \
		else { \
			EE_ERROR("Error: Tried setting a %s from a %s component in CopyFrom(Component*)", \
				this->GetName(), other ? other->GetName() : "null"); \
		} \
	} \
	virtual Elevate::GameObjectComponentFactory GetFactory() const override { \
		auto& entry = TypeRegistry::GetEntry<ThisType>(); \
		if (auto* trait = entry.GetTrait<::Elevate::ComponentTypeTrait>()) { \
			return trait->factory; \
		} \
		return nullptr; \
	} \
	virtual Elevate::GameObjectComponentDestructor GetDestructor() const override { \
		auto& entry = TypeRegistry::GetEntry<ThisType>(); \
		if (auto* trait = entry.GetTrait<::Elevate::ComponentTypeTrait>()) { \
			return trait->destructor; \
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

// =======================================================
// BEGIN_STRUCT / END_STRUCT
// =======================================================

#define BEGIN_STRUCT(T) \
private: \
	using ThisType = T; \
	inline static struct T##StructEntry { \
		T##StructEntry() { \
			::Elevate::TypeRegistry::AddClassToStack(#T); \
			FieldStartIndex = ::Elevate::TypeRegistry::CompilationClassFieldStack().size(); \
			StructName = ::Elevate::TypeRegistry::GetCleanedName(#T); \
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
			auto& global = ::Elevate::TypeRegistry::CompilationClassFieldStack(); \
			size_t start = generated_structEntry.FieldStartIndex; \
			for (size_t i = start; i < global.size(); ++i) { \
				generated_structEntry.StructFieldStack.push_back(global[i]); \
			} \
			if (start < global.size()) { \
				global.erase(global.begin() + start, global.end()); \
			} \
			::Elevate::TypeRegistry::GetCustomComponentFields()[typeid(ThisType)] = generated_structEntry.StructFieldStack; \
		} \
	} generated_structEntryEnd;
