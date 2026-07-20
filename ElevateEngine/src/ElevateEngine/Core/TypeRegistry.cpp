#include "eepch.h"
#include "TypeRegistry.h"

std::string Elevate::TypeRegistry::GetName(const std::type_info& type)
{
	auto& entries = GetEntries();
	auto it = entries.find(std::type_index(type));
	if (it != entries.end()) {
		return it->second.name;
	}
	else {
		return type.name();
	}
}

std::string Elevate::TypeRegistry::GetCleanedName(std::string rawName)
{
	std::string cleanedName = "";

	for (int i = 0; i < rawName.length(); i++)
	{
		char c = rawName.at(i); // Current char
		if (i == 0)
		{
			if (rawName.length() > 1 && (c == 'm' || c == 's' || c == 'g') && rawName.at(i + 1) == '_')
			{
				i = 1;
				continue;
			}
			cleanedName += std::toupper(c);
			continue;
		}

		bool lowerToUpper = std::islower(rawName.at(i - 1)) && std::isupper(c);
		bool acronymToEnd = std::isupper(c) && (i + 1 < rawName.length()) && std::islower(rawName.at(i + 1));
		if (lowerToUpper || acronymToEnd)
		{
			if (!cleanedName.empty() && cleanedName.back() != ' ')
			{
				cleanedName += ' ';
			}
		}

		if (c == '_')
		{
			cleanedName += ' ';
		}
		else if (rawName.at(i - 1) == '_')
		{
			cleanedName += std::toupper(c);
		}
		else
		{
			cleanedName += c;
		}
	}

	return cleanedName;
}

void Elevate::TypeRegistry::AddClassToStack(std::string newClass)
{
#ifdef EE_REGISTRY_LOG
	EE_CORE_INFO("{}", GetCleanedName(newClass));
#endif
	CompilationClassStack().push_back(newClass);
}

void Elevate::TypeRegistry::PopClassStack()
{
	if (CompilationClassStack().empty())
	{
		EE_CORE_ERROR("ERROR: Tried to PopClassStack but stack is empty!");
		return;
	}

	std::string fullName;
	for (int i = 0; i < CompilationClassStack().size(); i++)
	{
		fullName.append(CompilationClassStack()[i]);
		if (i != CompilationClassStack().size() - 1)
		{
			fullName.append("/");
		}
	}
	CompilationClassStack().pop_back();
	ClassPaths().push_back(fullName);
}
