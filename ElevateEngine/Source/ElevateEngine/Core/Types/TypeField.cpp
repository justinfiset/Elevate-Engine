module;

#include <typeindex>
#include <typeinfo>
#include <vector>

import Elevate.Core.Types.TypeField;
import Elevate.Core.Types.TypeRegistry;

namespace Elevate
{
	std::type_index TypeField::GetTargetType() const
	{
		auto& types = TypeRegistry::GetTypeIndexes();
		const auto it = types.find(targetTypeKey);

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

		const void* pointeeData = GetPointee(data);
		const auto pointeeType = GetPointeeTypeIndex(data);

		std::vector<TypeField> allFields;
		std::vector<TypeField> pointeeTypeField;

		for (const auto& child : children)
		{
			allFields.push_back(child);
		}

		auto& reflectedTypes = TypeRegistry::GetReflectedTypes();
		const auto reflectedIt = reflectedTypes.find(pointeeType);

		if (reflectedIt != reflectedTypes.end())
		{
			const auto& fields = reflectedIt->second;
			const auto* fieldData = reinterpret_cast<const TypeField*>(fields.data());
			pointeeTypeField.assign(fieldData, fieldData + fields.size());
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
