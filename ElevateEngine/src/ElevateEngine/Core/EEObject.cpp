#include "EEObject.h"

namespace Elevate
{
	constexpr size_t EEObject::GetGuidOffset()
	{
		return reinterpret_cast<size_t>(&(static_cast<EEObject*>(nullptr)->m_guid));
	}
		
	void EEObject::SetGuid(const Guid& guid)
	{
		m_guid = guid;
	}
}