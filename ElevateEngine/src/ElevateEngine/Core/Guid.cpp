#include "Guid.h"

#include <uuid.h>

namespace Elevate
{
	Guid Guid::New()
	{
		static std::random_device rd;
		static std::seed_seq seed{ rd(), rd(), rd(), rd() };
		static std::mt19937 generator(seed);
		uuids::uuid_random_generator gen(generator);
		uuids::uuid id = gen();

		Guid result;
		std::span<std::byte const, 16> bytes = id.as_bytes();
		std::copy(bytes.begin(), bytes.end(), result.m_bytes);
		return result;
	}

	std::string Guid::ToString() const
	{
		auto id = uuids::uuid(std::begin(m_bytes), std::end(m_bytes));
		return uuids::to_string(id);
	}

	ByteBuffer Guid::ToBytes() const
	{
		return ByteBuffer(std::begin(m_bytes), std::end(m_bytes));
	}

	bool Guid::operator==(const Guid& other) const
	{
		return std::equal(std::begin(m_bytes), std::end(m_bytes), std::begin(other.m_bytes));
	}
}