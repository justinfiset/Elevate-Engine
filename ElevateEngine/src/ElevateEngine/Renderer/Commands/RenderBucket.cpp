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
		if (m_commands.empty() || m_isSorted) return;

		// TODO Sort logic
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