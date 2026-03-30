#include "ProjectController.h"

#include <ElevateEngine/Core/Platform.h>

namespace EL
{
	void ProjectController::OpenDocumentation()
	{
		Elevate::Platform::OpenURL(std::string(DocumentationURL));
	}
}