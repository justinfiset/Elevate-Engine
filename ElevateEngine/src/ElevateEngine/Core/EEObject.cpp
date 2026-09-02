#include "EEObject.h"

#include <ElevateEngine/Core/Guid.h>

namespace Elevate
{
	void EEObject::SetGuid(const Guid& guid)
	{
		m_guid = guid;
	}
}