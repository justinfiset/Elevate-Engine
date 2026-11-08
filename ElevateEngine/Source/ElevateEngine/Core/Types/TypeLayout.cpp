module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

module Elevate.Core.Types.TypeLayout;

import Elevate.Foundations.Data;
import Elevate.Foundations.CoreLogger;

import Elevate.Core.Types.ValueCodec;

import Elevate.Serialization.PropertyField;

namespace Elevate
{
	TypeLayout::TypeLayout(const void* target, const std::string& name, const std::initializer_list<TypeField> fields)
		: m_objectInstance(target), m_name(name), m_fields(fields)
	{
		BindInstance(m_objectInstance);
	}

	TypeLayout::TypeLayout(const void* target, const std::string& name, const std::vector<TypeField>& fields)
		: m_objectInstance(target), m_name(name), m_fields(fields)
	{
		BindInstance(m_objectInstance);
	}

	void TypeLayout::BindFieldRecursively(TypeField& field, const void* parentAddress)
	{
		if (field.data == nullptr && parentAddress != nullptr)
		{
			field.data = static_cast<const char*>(parentAddress) + field.offset;
		}

		for (TypeField& child : field.children)
		{
			BindFieldRecursively(child, field.data);
		}
	}

	void TypeLayout::BindInstance(const void* instancePtr)
	{
		if (!instancePtr)
		{
			return;
		}

		m_objectInstance = instancePtr;

		for (TypeField& field : m_fields)
		{
			BindFieldRecursively(field, m_objectInstance);
		}
	}

	PropertyFlag GetFieldFlags(const TypeField& field)
	{
		return PropertyFlag::None;
	}

	namespace
	{
		TypeField MakeArrayElementField(const TypeField& array, const std::string& name, const void* elementData)
		{
			TypeField element;
			element.name = name;
			element.type = array.elementType;
			element.data = elementData;
			element.children = array.elementChildren;
			return element;
		}

		// ------------------------------------------------------------------
		// Capture : memory -> PropertySet
		// ------------------------------------------------------------------
		PropertyField CaptureField(const TypeField& field, const std::string& path, uint16_t depth);

		PropertySet CaptureChildren(const TypeField& parent, const std::string& parentPath, uint16_t depth)
		{
			PropertySet set;

			for (const TypeField& child : parent.children)
			{
				TypeField instance = child;
				if (!instance.data && parent.data)
				{
					instance.data = static_cast<const char*>(parent.data) + child.offset;
				}

				if (!instance.data && instance.type != EngineDataType::Array && instance.children.empty())
				{
					continue;
				}

				const std::string path = parentPath.empty() ? child.name : parentPath + "/" + child.name;
				set.push_back(CaptureField(instance, path, depth));
			}

			return set;
		}

		PropertySet CaptureArray(const TypeField& array, const std::string& path, uint16_t depth)
		{
			PropertySet set;

			if (!array.data || !array.GetArraySize || !array.GetElementAddress)
			{
				return set;
			}

			const size_t count = array.GetArraySize(array.data);
			for (size_t i = 0; i < count; ++i)
			{
				const void* elementData = array.GetElementAddress(array.data, i);
				if (!elementData)
				{
					continue;
				}

				const std::string indexName = "[" + std::to_string(i) + "]";
				const TypeField element = MakeArrayElementField(array, indexName, elementData);

				set.push_back(CaptureField(element, path + indexName, depth));
			}

			return set;
		}

		PropertyField CaptureField(const TypeField& field, const std::string& path, uint16_t depth)
		{
			PropertyField prop;
			prop.Name = field.name;
			prop.Type = field.type;
			prop.Path = path;
			prop.Depth = depth;
			prop.Flags = GetFieldFlags(field);

			if (field.type == EngineDataType::Array)
			{
				prop.Value = PropertyContainer{ CaptureArray(field, path, depth + 1) };
			}
			else if (!field.children.empty())
			{
				prop.Value = PropertyContainer{ CaptureChildren(field, path, depth + 1) };
			}
			else if (const ValueCodec* codec = ValueCodecRegistry::Find(field.type); codec && field.data)
			{
				codec->Capture(field.data, prop);
			}
			else
			{
				CoreLogger::Warn("CaptureState: no codec for '{}' (type {}, children {})",
					path, static_cast<int>(field.type), field.children.size());
			}

			return prop;
		}

		// ------------------------------------------------------------------
		// Apply : PropertySet -> memory
		// ------------------------------------------------------------------
		const PropertyField* FindByPath(const PropertySet& set, const std::string& path)
		{
			auto it = std::find_if(set.begin(), set.end(),
				[&path](const PropertyField& p) { return p.Path == path; });

			return it != set.end() ? &*it : nullptr;
		}

		void ApplyField(const TypeField& field, const PropertyField& prop, const std::string& path);

		void ApplyFields(const std::vector<TypeField>& fields, const void* parentData,
			const PropertySet& props, const std::string& parentPath)
		{
			for (const TypeField& field : fields)
			{
				const std::string path = parentPath.empty() ? field.name : parentPath + "/" + field.name;

				const PropertyField* prop = FindByPath(props, path);
				if (!prop)
				{
					continue;
				}

				TypeField instance = field;
				if (!instance.data && parentData)
				{
					instance.data = static_cast<const char*>(parentData) + field.offset;
				}

				ApplyField(instance, *prop, path);
			}
		}

		void ApplyArray(const TypeField& array, const PropertyField& prop)
		{
			const auto* container = std::get_if<PropertyContainer>(&prop.Value);
			if (!container || !array.data)
			{
				return;
			}

			if (array.ResizeArray)
			{
				array.ResizeArray(const_cast<void*>(array.data), container->Children.size());
			}

			const size_t count = array.GetArraySize ? array.GetArraySize(array.data) : 0;
			for (size_t i = 0; i < container->Children.size() && i < count; ++i)
			{
				const PropertyField& elementProp = container->Children[i];

				const void* elementData = array.GetElementAddress ? array.GetElementAddress(array.data, i) : nullptr;
				if (!elementData)
				{
					continue;
				}

				const TypeField element = MakeArrayElementField(array, elementProp.Name, elementData);
				ApplyField(element, elementProp, elementProp.Path);
			}
		}

		void ApplyField(const TypeField& field, const PropertyField& prop, const std::string& path)
		{
			if (field.type == EngineDataType::Array)
			{
				ApplyArray(field, prop);
			}
			else if (!field.children.empty())
			{
				if (const auto* container = std::get_if<PropertyContainer>(&prop.Value))
				{
					ApplyFields(field.children, field.data, container->Children, path);
				}
				else
				{
					CoreLogger::Warn("ApplyState: '{}' should hold a container", path);
				}
			}
			else if (const ValueCodec* codec = ValueCodecRegistry::Find(field.type); codec && field.data)
			{
				if (!codec->Apply(const_cast<void*>(field.data), prop))
				{
					CoreLogger::Warn("ApplyState: failed to restore '{}' (type {})", path, static_cast<int>(field.type));
				}
			}
			else
			{
				CoreLogger::Warn("ApplyState: no codec for '{}' (type {}, children {})",
					path, static_cast<int>(field.type), field.children.size());
			}
		}
	}

	PropertySet TypeLayout::CaptureState() const
	{
		PropertySet set;

		for (const TypeField& field : m_fields)
		{
			if (!field.data && field.type != EngineDataType::Array && field.children.empty())
			{
				continue;
			}

			set.push_back(CaptureField(field, field.name, 0));
		}

		return set;
	}

	void TypeLayout::ApplyState(const PropertySet& props)
	{
		ApplyFields(m_fields, m_objectInstance, props, "");
	}
}
