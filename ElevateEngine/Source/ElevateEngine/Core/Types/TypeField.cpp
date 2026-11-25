module;

#include <map>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

module Elevate.Core.Types.TypeField;

namespace Elevate
{
	std::unordered_map<const char*, std::type_index>& GetTypeIndexes()
	{
		static std::unordered_map<const char*, std::type_index> entries;
		return entries;
	}

	std::map<std::type_index, std::vector<TypeField>>& GetReflectedTypes()
	{
		static std::map<std::type_index, std::vector<TypeField>> fields;
		return fields;
	}

	std::type_index TypeField::GetTargetType() const
	{
		auto& types = GetTypeIndexes();
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

		auto& reflectedTypes = GetReflectedTypes();
		const auto reflectedIt = reflectedTypes.find(pointeeType);

		if (reflectedIt != reflectedTypes.end())
		{
			const auto& fields = reflectedIt->second;
			pointeeTypeField.assign(fields.begin(), fields.end());
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
