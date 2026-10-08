module;

#include <initializer_list>
#include <vector>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <set>
#include <format>
#include <map>
#include <variant>

#include <entt/entt.hpp>
#include <glm/fwd.hpp>

export module Elevate.Core.Types.TypeRegistry;

import Elevate.Foundations.Data;
import Elevate.Foundations.Enums;

import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;

import Elevate.Core.Types.TypeField;
import Elevate.Core.Types.ITypeTrait;

import Elevate.Core.Reflection.ReflectionTags;

namespace Elevate
{
	class Component;
	class GameObject;
}

#ifdef EE_EDITOR_BUILD
import Elevate.Editor.Types.EditorTypeTrait;
#endif

export namespace Elevate
{
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
	struct is_engine_array<std::vector<T, Alloc>> : std::true_type {};
	template<typename T, typename Alloc>
	struct is_engine_array<std::set<T, Alloc>> : std::true_type {};
	template<typename T>
	inline constexpr bool is_engine_array_v = is_engine_array<T>::value;

	template<typename T>
	struct ee_ptr_target { using type = void; };
	template<typename T>
	struct ee_ptr_target<EEObjectPtr<T>> { using type = T; };
	template<typename T>
	using ee_ptr_target_t = typename ee_ptr_target<T>::type;

	template<typename T>
	struct is_ee_object_ptr : std::false_type {};
	template<typename T>
	struct is_ee_object_ptr<EEObjectPtr<T>> : std::true_type {};
	template<typename T>
	inline constexpr bool is_ee_object_ptr_v = is_ee_object_ptr<T>::value;

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
	template<typename T>
	struct EngineDataTypeTrait<T, std::enable_if_t<is_ee_object_ptr_v<T>>>
	{
		static constexpr EngineDataType value = EngineDataType::ObjectPtr;
	};
	template<typename T>
	struct EngineDataTypeTrait<T, std::enable_if_t<std::is_enum_v<T>>>
	{
		static constexpr EngineDataType value = EngineDataType::Enum;
	};

	using ObjectFactory = std::function<std::shared_ptr<EEObject>()>;

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
			ObjectFactory factory = nullptr;

			Entry() = default;
			Entry(const std::string& name, std::type_index& type)
				: name(name), type(type) {
			}

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

		static std::unordered_map<const void*, std::type_index>& GetTypeIndexes()
		{
			static std::unordered_map<const void*, std::type_index> entries;
			return entries;
		}

		static Entry& GetEntry(std::type_index id)
		{
			return GetEntries()[id];
		}

		template<typename T>
		static Entry& GetEntry()
		{
			return GetEntry(typeid(T));
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

		template<typename T>
		static void Register(const std::string& name, const std::vector<FieldOption>& options)
		{
			// Register in the type_index -> Entry relation map
			std::type_index ti(typeid(T));
			auto& entry = GetEntries()[ti];
			entry.name = name;
			entry.type = ti;

			// Register the T -> type_index relation map
			GetTypeIndexes().insert_or_assign(GetTypeKey<T>(), ti);

			if constexpr (std::is_base_of_v<EEObject, T> && !std::is_abstract_v<T> && std::is_default_constructible_v<T>)
			{
				entry.factory = []() -> std::shared_ptr<EEObject>
					{
						return std::make_shared<T>();
					};
			}

#ifdef EE_EDITOR_BUILD
			// Passing T using a type_identity simple placeholder.
			AddTrait<T, EditorTypeTrait>(std::type_identity<T>{}, options);
#endif
		}

		template<typename Class, typename FieldType>
		static void AddPropertyDirect(
			FieldType Class::* member,
			const std::string& name,
			std::initializer_list<FieldOption> options,
			std::vector<TypeField>& targetStack
		) {
			using CleanedFieldT = std::decay_t<FieldType>;
			constexpr EngineDataType type = DeduceEngineDataType<CleanedFieldT>();

			FieldMeta meta;
			std::string cleanedName = GetCleanedName(name);
			meta.displayName = cleanedName;
			for (auto&& opt : options) {
				if (std::holds_alternative<FlattenTag>(opt)) { meta.flatten = true; }
				else if (std::holds_alternative<DisplayNameTag>(opt)) { meta.displayName = std::get<DisplayNameTag>(opt).value; }
				else if (std::holds_alternative<TooltipTag>(opt)) { meta.tooltip = std::get<TooltipTag>(opt).text; }
				else if (std::holds_alternative<ReadOnlyTag>(opt)) { meta.readOnly = true; }
				else if (std::holds_alternative<ColorTag>(opt)) { meta.isColor = true; }
			}

			alignas(Class) char dummyBuffer[sizeof(Class)];
			Class* dummyObj = reinterpret_cast<Class*>(dummyBuffer);
			size_t offset = static_cast<size_t>(
				reinterpret_cast<const char*>(&(dummyObj->*member)) - dummyBuffer
				);

			TypeField field;

			if constexpr (is_engine_array_v<CleanedFieldT>)
			{
				field = TypeField(name, EngineDataType::Array, offset, meta.displayName);

				using ElementType = typename CleanedFieldT::value_type;
				field.elementType = DeduceEngineDataType<ElementType>();

				if constexpr (is_ee_object_ptr_v<ElementType>)
				{
					using TargetT = ee_ptr_target_t<ElementType>;
					field.targetTypeKey = GetTypeKey<TargetT>();
				}
				else if constexpr (std::is_class_v<ElementType> && !std::is_same_v<ElementType, std::string>)
				{
					auto& customFields = GetReflectedTypes();
					std::type_index ti = typeid(ElementType);
					auto it = customFields.find(ti);
					if (it != customFields.end()) {
						field.elementChildren = it->second;
					}
				}

				field.GetArraySize = [](const void* vecPtr) -> size_t {
					if (!vecPtr) return 0;
					const auto* vec = static_cast<const CleanedFieldT*>(vecPtr);
					return vec->size();
					};

				field.GetElementAddress = [](const void* vecPtr, size_t index) -> const void* {
					if (!vecPtr) return nullptr;
					const auto* vec = static_cast<const CleanedFieldT*>(vecPtr);
					if (index >= vec->size()) return nullptr;
					return static_cast<const void*>(&(*vec)[index]);
					};

				field.ResizeArray = [](void* vecPtr, size_t newSize) {
					if (!vecPtr) return;
					auto* vec = static_cast<CleanedFieldT*>(vecPtr);
					vec->resize(newSize);
					};
			}
			else if constexpr (is_ee_object_ptr_v<CleanedFieldT>)
			{
				field = TypeField(name, EngineDataType::ObjectPtr, offset, meta.displayName);
				using TargetT = ee_ptr_target_t<CleanedFieldT>;
				field.targetTypeKey = GetTypeKey<TargetT>();
			}
			else if (type == EngineDataType::Enum)
			{
				field = TypeField(name, type, offset, meta.displayName);
				field.targetTypeKey = GetTypeKey<CleanedFieldT>();
			}
			else if (type == EngineDataType::Custom)
			{
				auto& customFields = GetReflectedTypes();
				std::type_index ti = typeid(CleanedFieldT);

				std::vector<TypeField> subFields;
				auto it = customFields.find(ti);
				if (it != customFields.end()) {
					subFields = it->second;
				}
				field = TypeField(name, EngineDataType::Custom, offset, meta.displayName, subFields);
			}
			else
			{
				field = TypeField(name, type, offset, meta.displayName);
			}

			field.flatten = meta.flatten;
			field.isColor = meta.isColor;
			field.tooltip = meta.tooltip;
			field.readOnly = meta.readOnly;

			targetStack.push_back(field);
		}

		// Enums
		struct EnumValue
		{
			std::string Name;
			EnumType Value;
		};

		struct EnumInfo
		{
			std::string Name;
			std::vector<EnumValue> Values;
		};

		template<typename T>
		static const void* GetTypeKey()
		{
			static const char key{};
			return &key;
		}

		template<typename T>
		static void RegisterEnum(const char* name, std::initializer_list<EnumValue> values)
		{
			static_assert(std::is_enum_v<T>);

			std::type_index ti = typeid(T);

			EnumInfo info;
			info.Name = name;
			info.Values = values;

			// Register in the type_index -> Entry relation map
			GetEnums()[ti] = std::move(info);

			// Register the T -> type_index relation map
			GetTypeIndexes().insert_or_assign(GetTypeKey<T>(), ti);
		}

		static std::unordered_map<std::type_index, EnumInfo>& GetEnums()
		{
			static std::unordered_map<std::type_index, EnumInfo> enums;
			return enums;
		}

		static const EnumInfo* GetEnum(std::type_index typeIndex)
		{
			auto& enums = GetEnums();

			auto it = enums.find(typeIndex);
			if (it != enums.end())
			{
				return &it->second;
			}
			return nullptr;
		}

		template<typename T>
		static const EnumInfo* GetEnum()
		{
			return GetEnum(typeid(T));
		}
	};
}