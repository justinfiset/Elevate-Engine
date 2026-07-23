#pragma once

// Interfaces
#include <ElevateEngine/Serialization/ISerializable.h>
#include <ElevateEngine/Core/ITypeLayoutProvider.h>

#include <ElevateEngine/Core/Guid.h>
#include <ElevateEngine/Core/EECategory.h>

#include <ElevateEngine/Core/TypeLayout.h>
#include <ElevateEngine/Serialization/PropertyField.h>

#include <typeindex>

namespace Elevate
{
	class EEObject : public ISerializable, public ITypeLayoutProvider
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
		inline virtual std::string GetName() const { return "EEObject"; }
		inline virtual TypeLayout GetLayout() const { return TypeLayout(GetName(), {}); }
		PropertySet GetProperties() const { return GetLayout().CaptureState(); }
		inline virtual std::type_index GetTypeIndex() const { return typeid(EEObject); }

		// ISerializable Implementation
		virtual ByteBuffer Serialize() const override;
		virtual void Deserialize(const ByteBuffer& data) override;
	};
}