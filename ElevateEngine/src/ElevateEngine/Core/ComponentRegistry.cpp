#include "eepch.h"
#include "ComponentRegistry.h"

void Elevate::ComponentRegistry::AddClassToStack(std::string newClass)
{
    EE_CORE_INFO(GetCleanedName(newClass));
    CompilationClassStack().push_back(newClass);
}

void Elevate::ComponentRegistry::PopClassStack()
{
    if (CompilationClassStack().empty()) {
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
