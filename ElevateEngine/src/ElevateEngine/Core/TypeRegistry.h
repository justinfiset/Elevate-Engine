#pragma once

#include <initializer_list>
#include <map>
#include <vector>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

#include <entt/entt.hpp>
#include <glm/fwd.hpp>

#include <ElevateEngine/Core/ReflectionTags.h>
#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/EECategory.h>
// todo remove in the near futur
#include <ElevateEngine/Core/TypeLayout.h>

#ifdef EE_RELEASE
	#undef EE_REGISTRY_LOG
#endif

namespace Elevate
{
	class Component;
	class GameObject;

	template<typename T, typename = void>
	struct has_super : std::false_type {};
	template<typename T>
	struct has_super<T, std::void_t<typename T::Super>> : std::true_type {};

	template<typename T, bool = has_super<T>::value>
	struct ParentFieldsHelper {
		static std::vector<TypeField> Get() { return {}; }
	};

	template<typename T>
	struct ParentFieldsHelper<T, true> {
		static std::vector<TypeField> Get() {
			using Super = typename T::Super;
			if constexpr (!std::is_same_v<Super, void> &&
				std::is_base_of_v<Component, Super> &&
				!std::is_same_v<Super, Elevate::Component>) {
				return Super::generated_classEntry.ClassFieldStack;
			}
			else {
				return {};
			}
		}
	};

	// Prevent recursion when facing the Component class
	template<>
	struct ParentFieldsHelper<Elevate::Component, true> {
		static std::vector<Elevate::TypeField> Get() {
			return {};
		}
	};
	template<>
	struct ParentFieldsHelper<Elevate::Component, false> {
		static std::vector<Elevate::TypeField> Get() {
			return {};
		}
	};

	struct FieldMeta {
		bool flatten = false;
		std::string displayName = "";
		std::string tooltip;
		bool readOnly = false;
		bool isColor = false;
		std::string IconPath;
	};


	template<typename T>
	struct is_engine_array : std::false_type {};
	template<typename T, typename Alloc>
	struct is_engine_array : std::true_type {};
	template<typename T>
	inline constexpr bool is_engine_array_v = is_engine_array<T>::value;

	template<typename T, typename = void>
	struct EngineDataTypeTrait
	{
		static constexpr EngineDataType value = EngineDataType::Custom;
	};

	template<> struct EngineDataTypeTrait<float> { static constexpr EngineDataType value = EngineDataType::Float; };
	template<> struct EngineDataTypeTrait<double> { static constexpr EngineDataType value = EngineDataType::Double; };
	template<> struct EngineDataTypeTrait<int> { static constexpr EngineDataType value = EngineDataType::Int; };
	template<> struct EngineDataTypeTrait<bool> { static constexpr EngineDataType value = EngineDataType::Bool; };
	template<> struct EngineDataTypeTrait<glm::vec2> { static constexpr EngineDataType value = EngineDataType::Float2; };
	template<> struct EngineDataTypeTrait<glm::vec3> { static constexpr EngineDataType value = EngineDataType::Float3; };
	template<> struct EngineDataTypeTrait<glm::vec4> { static constexpr EngineDataType value = EngineDataType::Float4; };
	template<> struct EngineDataTypeTrait<std::string> { static constexpr EngineDataType value = EngineDataType::String; };
	template<typename T> struct EngineDataTypeTrait<T, std::enable_if_t<is_engine_array_v<T>>>
	{
		static constexpr EngineDataType value = EngineDataType::Array;
	};

	struct ITypeTrait
	{
		virtual ~ITypeTrait() = default;
	};

	using GameObjectComponentGetter = std::function<Component* (std::weak_ptr<GameObject>)>;
	using GameObjectComponentFactory = std::function<Component* (std::weak_ptr<GameObject>)>;
	using GameObjectComponentDestructor = std::function<void(std::weak_ptr<GameObject>)>;

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
		static auto GetParentFieldsIfPossible(const T* obj) -> std::vector<TypeField> {
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

		static inline std::vector<TypeField>& CompilationClassFieldStack()
		{
			static std::vector<TypeField> stack;
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

		static std::map<std::type_index, std::vector<TypeField>>& GetReflectedTypes()
		{
			static std::map<std::type_index, std::vector<TypeField>> m_customComponentFields;
			return m_customComponentFields;
		}

		template<typename Class, typename FieldType>
		static void AddPropertyDirect(
			FieldType Class::* member,
			const std::string& name,
			std::initializer_list<FieldOption> options,
			std::vector<TypeField>& targetStack
		);
	};
}

#ifdef EE_EDITOR_BUILD
	#include <ElevateEngine/Editor/EditorTypeTrait.h>
#endif

#include "TypeRegistry.inl"