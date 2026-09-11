#pragma once
#include <memory>
#include <type_traits>

#include <ElevateEngine/Core/Byte.h>
#include <ElevateEngine/Core/Guid.h>
#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Serialization/ISerializable.h>

namespace Elevate
{
	namespace Detail
	{
		std::shared_ptr<EEObject> ResolveAssetHelper(const Guid& guid);
	}

	template<typename T>
	class EEObjectPtr : public ISerializable
	{
	private:
		// Allow other type to interact with other's private members
		template<typename U>
		friend class EEObjectPtr;

		mutable std::shared_ptr<T> m_ptr = nullptr;
		mutable Guid m_guid{};

	private:
		void UpdateGuid() const
		{
			if (m_ptr)
			{
				m_guid = m_ptr->GetGuid();
			}
			else
			{
				m_guid = Guid{};
			}
		}

	public:
		EEObjectPtr() : m_ptr(nullptr), m_guid(Guid{})
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
		}
		EEObjectPtr(std::nullptr_t) noexcept : m_ptr(nullptr), m_guid(Guid{}) { }
		EEObjectPtr(std::shared_ptr<T> ptr) : m_ptr(std::move(ptr))
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
			UpdateGuid();
		}
		EEObjectPtr(const EEObjectPtr<T>& other) : m_ptr(other.m_ptr), m_guid(other.m_guid) { }

		explicit EEObjectPtr(const Guid& guid) : m_ptr(nullptr), m_guid(guid)
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
		}

		void SetGuid(const Guid& guid)
		{
			if (m_guid != guid)
			{
				m_guid = guid;
				m_ptr = nullptr;
			}
		}

		const Guid& GetGuid()
		{
			return m_guid;
		}

		virtual ByteBuffer Serialize() const override;
		// todo : virtual void Deserialize(const ByteBuffer& data);

		T* get() const
		{
			if (m_ptr)
			{
				return m_ptr.get();
			}
			else if (m_guid.IsValid())
			{
				auto rawAsset = Detail::ResolveAssetHelper(m_guid);
				m_ptr = std::static_pointer_cast<T>(rawAsset);
				UpdateGuid();
				if (m_ptr)
				{
					return m_ptr.get();
				}
			}
			return nullptr;
		}

		T* operator->() const noexcept
		{
			return get();
		}

		T& operator*() const noexcept
		{
			return *get();
		}

		explicit operator bool() const noexcept
		{
			return get() != nullptr;
		}

		bool operator==(const EEObjectPtr<T>& other) const noexcept
		{
			return m_guid == other.m_guid;
		}

		bool operator==(const std::shared_ptr<T>& other) const noexcept
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");

			if (!other)
			{
				return !m_guid.IsValid();
			}
			return m_guid == other->GetGuid();
		}

		template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
		EEObjectPtr(const EEObjectPtr<U>& other)
			: m_ptr(other.m_ptr), m_guid(other.m_guid)
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
		}

		template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
		EEObjectPtr<T>& operator=(const EEObjectPtr<U>& other)
		{
			if ((void*)this != (void*)&other)
			{
				m_ptr = other.m_ptr;
				m_guid = other.m_guid;
			}
			return *this;
		}

		EEObjectPtr<T>& operator=(const EEObjectPtr<T>& other) {
			if (this != &other)
			{
				m_ptr = other.m_ptr;
				m_guid = other.m_guid;
			}
			return *this;
		}

		EEObjectPtr<T>& operator=(std::shared_ptr<T> ptr) {
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
			m_ptr = std::move(ptr);
			UpdateGuid();
			return *this;
		}

		EEObjectPtr<T>& operator=(std::nullptr_t) noexcept {
			m_ptr.reset();
			UpdateGuid();
			return *this;
		}

		void reset(std::shared_ptr<T> ptr)
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
			m_ptr = std::move(ptr);
			UpdateGuid();
		}

		void reset(T* rawPtr)
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
			if (rawPtr)
			{
				m_ptr = std::static_pointer_cast<T>(rawPtr->shared_from_this());
			}
			else
			{
				m_ptr.reset();
			}
			UpdateGuid();
		}

		void reset(const T* rawPtr)
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
			if (rawPtr)
			{
				auto nonConst = const_cast<T*>(rawPtr);
				m_ptr = std::static_pointer_cast<T>(nonConst->shared_from_this());		
			}
			else
			{
				m_ptr.reset();
			}
			UpdateGuid();
		}
			
		void reset() noexcept
		{
			m_ptr.reset();
			UpdateGuid();
		}

		bool operator<(const EEObjectPtr<T>& other) const noexcept
		{
			return m_guid < other.m_guid;
		}

		std::weak_ptr<T> ToWeak() const noexcept
		{
			return std::weak_ptr<T>(m_ptr);
		}

		std::shared_ptr<T> ToShared() const
		{
			get(); // Force the proxy to resolve the object if pointer is invalid
			return m_ptr;
		}

		operator std::shared_ptr<T>() const
		{
			return ToShared();
		}
	};

	template<typename T>
	inline ByteBuffer EEObjectPtr<T>::Serialize() const
	{
		if (m_ptr)
		{
			// We already verified that the type is derived from EEObject, therefore this is safe.
			const EEObject* base = (const EEObject*)m_ptr.get();
			return base->GetGuid().ToBytes();
		}
		return m_guid.ToBytes();
	}
}

//#include "EEObjectPtr.inl"