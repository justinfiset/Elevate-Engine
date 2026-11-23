module;

#include <typeindex>
#include <typeinfo>
#include <vector>

#include <ElevateEngine/Core/Types/TypeRegistryBridge.h>

module Elevate.Core.Types.TypeField;

namespace Elevate
{
	std::type_index Elevate::TypeField::GetTargetType() const
	{
		const auto* typeIndex = Internal::FindTypeIndex(targetTypeKey);
		return typeIndex ? *typeIndex : typeid(void);
	}

	void TypeField::UseInstance(const void* instancePtr)
	{
		data = reinterpret_cast<const char*>(instancePtr) + offset;

		if (!GetPointee)
		{
			return;
		}

		// If we are in the presence of a pointer, adjust the child fields with the right adress
		const void* pointeeData = GetPointee(data);
		const auto pointeeType = GetPointeeTypeIndex(data);

		std::vector<TypeField> allFields;
		std::vector<TypeField> pointeeTypeField;

		for (const auto& child : children)
		{
			allFields.push_back(child);
		}

		std::size_t reflectedFieldCount = 0;
		const auto* reflectedFields = static_cast<const TypeField*>(
			Internal::GetReflectedFieldData(pointeeType, reflectedFieldCount));

		if (reflectedFields)
		{
			pointeeTypeField.assign(reflectedFields, reflectedFields + reflectedFieldCount);
		}

		for (const auto& pointeeField : pointeeTypeField)
		{
			allFields.push_back(pointeeField);
		}

		children.clear();

		children.reserve(allFields.size());

		for (const auto& field : allFields)
		{
			const void* childData = static_cast<const void*>(static_cast<const char*>(pointeeData) + field.offset);
			children.emplace_back(field, childData);
		}
	}
}
