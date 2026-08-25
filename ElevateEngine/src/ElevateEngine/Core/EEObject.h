#pragma once

#include <typeindex>

// Interfaces
#include <ElevateEngine/Core/ITypeLayoutProvider.h>

#include <ElevateEngine/Core/Guid.h>
#include <ElevateEngine/Core/EECategory.h>

#include <ElevateEngine/Core/TypeLayout.h>
#include <ElevateEngine/Serialization/PropertyField.h>

namespace Elevate
{
	class EEObject : public ITypeLayoutProvider
	{
	protected:
		EECategory m_category;
		Guid m_guid;

	public:
		EEObject() : m_guid(Guid::New()) { }
		EEObject(EECategory category)
			: m_category(category), m_guid(Guid::New()) { }
		virtual ~EEObject() = default;

		virtual EECategory GetCategory() const { return m_category; }
		const Guid& GetGuid() const { return m_guid; }

		// If no overrode, an empty layout is generated and nothing is shown in the inspector
		virtual std::string GetName() const { return "EEObject"; }
		virtual TypeLayout GetLayout() const { return TypeLayout(nullptr, GetName(), {}); }
		PropertySet GetProperties() const { return GetLayout().CaptureState(); }
		virtual std::type_index GetTypeIndex() const { return typeid(EEObject); }
	};
}