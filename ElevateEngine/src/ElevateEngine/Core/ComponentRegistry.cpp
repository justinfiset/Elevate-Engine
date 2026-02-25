#include "eepch.h"
#include "ComponentRegistry.h"

std::string Elevate::ComponentRegistry::GetName(const std::type_info& type)
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

std::string Elevate::ComponentRegistry::GetCleanedName(std::string rawName)
{
    std::string cleanedName = "";

    for (int i = 0; i < rawName.length(); i++)
    {
        char c = rawName.at(i); // Current char
        if (i == 0)
        {
            if ((c == 'm' || c == 's' || c == 'g') && rawName.at(i + 1) == '_') // If we have a notation of Hungarian style : m_myVariable
            {
                i = 1;
                continue;
            }
            else
            {
                cleanedName += std::toupper(c);
            }
        }
        else if (rawName.length() > i + 1 && std::isupper(c) && std::islower(rawName.at(i + 1)))
        {
            cleanedName += ' ';
            cleanedName += c;
        }
        else if (std::isalpha(c) && rawName.at(i - 1) == '_')
        {
            cleanedName += std::toupper(c);
        }
        else if (c == '_')
        {
            cleanedName += ' ';
        }
        else
        {
            cleanedName += c;
        }
    }

    return cleanedName;
}

void Elevate::ComponentRegistry::AddClassToStack(std::string newClass)
{
#ifdef EE_REGISTRY_LOG
    EE_CORE_INFO("{}", GetCleanedName(newClass));
#endif
    CompilationClassStack().push_back(newClass);
}

void Elevate::ComponentRegistry::PopClassStack()
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
