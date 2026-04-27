#pragma once
#include <memory>

#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Serialization/ISerializable.h>

namespace Elevate
{
	template<typename T>
	class EEObjectPtr : public ISerializable
	{
	private:
		std::shared_ptr<T> m_ptr;

	public:
		EEObjectPtr() : m_ptr(nullptr)
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
		}
		EEObjectPtr(std::nullptr_t) noexcept : m_ptr(nullptr) { }
		EEObjectPtr(std::shared_ptr<T> ptr) : m_ptr(std::move(ptr))
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
		}
		EEObjectPtr(const EEObjectPtr<T>& other) : m_ptr(other.m_ptr) {}

		virtual ByteBuffer Serialize() const override;
		// todo : virtual void Deserialize(const ByteBuffer& data);

		T* operator->() const noexcept
		{
			return m_ptr.get();
		}

		T& operator*() const noexcept
		{
			return *m_ptr;
		}

		explicit operator bool() const noexcept
		{
			return m_ptr != nullptr;
		}

		bool operator==(const EEObjectPtr<T>& other) const noexcept
		{
			return m_ptr == other.m_ptr;
		}

		bool operator==(const std::shared_ptr<T>& other) const noexcept
		{
			return m_ptr == other;
		}

		EEObjectPtr<T>& operator=(const EEObjectPtr<T>& other) {
			if (this != &other) m_ptr = other.m_ptr;
			return *this;
		}

		EEObjectPtr<T>& operator=(std::shared_ptr<T> ptr) {
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
			m_ptr = std::move(ptr);
			return *this;
		}

		EEObjectPtr<T>& operator=(std::nullptr_t) noexcept {
			m_ptr.reset();
			return *this;
		}

		T* get() const noexcept { return m_ptr.get(); }

		void reset(std::shared_ptr<T> ptr)
		{
			static_assert(std::is_base_of_v<EEObject, T>, "T must derive from EEObject");
			m_ptr = std::move(ptr);
		}

		void reset() noexcept
		{
			m_ptr.reset();
		}

		bool operator<(const EEObjectPtr<T>& other) const noexcept
		{
			return m_ptr < other.m_ptr;
		}

		std::weak_ptr<T> ToWeak() const noexcept
		{
			return std::weak_ptr<T>(m_ptr);
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
		return ByteBuffer(16);
	}
}