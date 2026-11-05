module;

#include <typeindex>
#include <typeinfo>
#include <vector>

module Elevate.Core.Types.TypeField;

import Elevate.Core.Types.TypeRegistry;

namespace Elevate
{
	std::type_index Elevate::TypeField::GetTargetType() const
	{
		auto& types = TypeRegistry::GetTypeIndexes();
		auto it = types.find(targetTypeKey);

		if (it == types.end())
		{
			return typeid(void);
		}

		return it->second;
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

		const auto& customFields = TypeRegistry::GetReflectedTypes();
		auto it = customFields.find(pointeeType);

		if (it != customFields.end())
		{
			pointeeTypeField = it->second;
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
