#include "Guid.h"

#include <stdint.h>
#include <uuid.h>
#include <algorithm>
#include <cstring>
#include <format>
#include <iterator>
#include <optional>
#include <random>
#include <span>
#include <string>

#include <ElevateEngine/Core/Log.h>
#include "ElevateEngine/Core/Byte.h"

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

	Guid Guid::FromString(const std::string& str)
	{
		auto idOpt = uuids::uuid::from_string(str);
		if (!idOpt.has_value())
		{
			EE_CORE_ERROR("Failed to parse guid from string : {}", str);
			return Guid{}; // Return an ampty guid
		}

		Guid result;
		std::span<std::byte const, 16> bytes = idOpt->as_bytes();
		std::copy(bytes.begin(), bytes.end(), result.m_bytes);
		return result;
	}

	std::string Guid::ToString() const
	{
		auto beginPtr = reinterpret_cast<const uint8_t*>(m_bytes);
		auto id = uuids::uuid(beginPtr, beginPtr + 16);
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

	bool Guid::IsValid() const
	{
		static const Guid nullGuid{};
		return std::memcmp(m_bytes, nullGuid.m_bytes, sizeof(m_bytes)) != 0;
	}

	std::size_t Guid::GetHash() const noexcept
	{
		uint64_t high, low;
		std::memcpy(&high, m_bytes, sizeof(uint64_t));
		std::memcpy(&low, m_bytes + 8, sizeof(uint64_t));
		std::size_t h1 = std::hash<uint64_t>{}(high);
		std::size_t h2 = std::hash<uint64_t>{}(low);
		return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
	}

	bool Guid::operator!=(const Guid& other) const
	{
		return !(*this == other);
	}

	bool Guid::operator<(const Guid& other) const
	{
		return std::memcmp(m_bytes, other.m_bytes, sizeof(m_bytes)) < 0;
	}
}