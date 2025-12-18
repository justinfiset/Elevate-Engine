#include "RenderBucket.h"
#include "RenderCommandQueue.h"

namespace Elevate
{
	void RenderBucket::Submit(const RenderCommand& command)
	{
		m_commands.push_back(command);
		m_isSorted = false;
	}

	void RenderCommandQueue::Submit(RenderBucket::Type bucketType, std::shared_ptr<VertexArray>& array, const glm::mat4& transform, const RenderState& state)
	{
		m_buckets->at((size_t)bucketType).Submit(
			RenderCommand(array, transform, state)
		);
	}

	void RenderBucket::Sort()
	{
		// todo test this sorting algorithm performance against std::sort
		// also test the optimal bit shift count (bit, 3 bits, byte ...)
		// todo find optimal type to limit memory usage instead of ints
		if (m_commands.empty() || m_isSorted) return;

		std::vector<RenderCommand> output;

		uint16_t maxShift = sizeof(m_commands[0].SortKey) * 8;
		int bitCount = 1;
		for (uint16_t shift = 0; shift < maxShift / bitCount; shift++)
		{
			int count[1 << bitCount];
			output.resize(m_commands.size());

			for (const RenderCommand& command : m_commands)
			{
				count[(command.SortingKey >> shift * bitCount) & ((1 << bitCount) - 1)]++;
			}
			for (int i = 0; i < (1 << bitCount); i++)
			{
				count[i] += count[i - 1];
			}
			for (int i = static_cast<int>(m_commands.size()) - 1; i >= 0; i--)
			{
				output[--count[(m_commands[i].SortingKey >> shift * bitCount) & ((1 << bitCount) - 1)]] = m_commands[i];
			}
			m_commands = output;
		}

		m_isSorted = true;
	}

	void RenderCommandQueue::FlushAll()
	{
		for (size_t i = 0; i < m_buckets->size(); i++)
		{
			m_buckets->at(i).Flush();
		}
		Clear();
	}

	const RenderBucket& RenderCommandQueue::GetBucket(RenderBucket::Type bucketType) const
	{
		return m_buckets->at((size_t) bucketType);
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