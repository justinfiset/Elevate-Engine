#include "RenderBucket.h"
#include <ElevateEngine/Renderer/Renderer.h>

namespace Elevate
{
	void RenderBucket::Submit(const RenderCommand& command)
	{
		m_commands.push_back(command);
		m_isSorted = false;
	}

	void RenderBucket::Flush()
	{
		if (m_commands.empty()) return;

		if (!m_isSorted)
		{
			Sort();
		}

		// Sends each command to be executed by the renderer
		for (const auto& command : m_commands)
		{
			Renderer::Dispatch(command);
		}

		Clear();
	}

	void RenderBucket::Clear()
	{
		m_commands.clear();
		m_isSorted = false;
	}

	void RenderBucket::Sort()
	{
		// todo also test the optimal bit shift count (bit, 3 bits, byte ...)
		// todo find optimal type to limit memory usage instead of ints
		if (m_commands.empty() || m_isSorted) return;

		const size_t n = m_commands.size();
		std::vector<RenderCommand> tempBuffer(n);

		RenderCommand* src = m_commands.data();
		RenderCommand* dst = tempBuffer.data();

		const int bitsPerPass = 8;
		const int bucketCount = 1 << bitsPerPass;
		const uint32_t mask = bucketCount - 1;
		const uint16_t keySizeBits = sizeof(m_commands[0].SortingKey) * 8;

		for (uint16_t shift = 0; shift < keySizeBits; shift += bitsPerPass)
		{
			uint32_t count[bucketCount] = { 0 };

			for (size_t i = 0; i < n; i++)
			{
				count[(src[i].SortingKey >> shift) & mask]++;
			}

			for (int i = 1; i < bucketCount; ++i)
			{
				count[i] += count[i - 1];
			}

			for (long long i = (long long)m_commands.size() - 1; i >= 0; --i)
			{
				uint32_t bucket = (src[i].SortingKey >> shift) & mask;
				dst[--count[bucket]] = src[i];
			}

			std::swap(src, dst);
		}

		if (src != m_commands.data())
		{
			std::copy(tempBuffer.begin(), tempBuffer.end(), m_commands.begin());
		}

		m_isSorted = true;
	}

	size_t RenderBucket::GetCommandCount() const
	{
		return m_commands.size();
	}

	size_t RenderBucket::GetMemoryUsage() const
	{
		return GetCommandCount() * sizeof(RenderCommand);
	}
}