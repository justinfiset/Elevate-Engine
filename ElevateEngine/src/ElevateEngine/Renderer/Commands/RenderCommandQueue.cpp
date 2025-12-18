#include "RenderCommandQueue.h"

namespace Elevate
{
	void RenderCommandQueue::Submit(RenderBucket::Type bucketType, std::shared_ptr<VertexArray>& array, const glm::mat4& transform, const RenderState& state)
	{
		m_buckets->at((size_t)bucketType).Submit(
			RenderCommand(array, transform, state)
		);
	}

	void RenderCommandQueue::Clear()
	{
		for (size_t i = 0; i < m_buckets->size(); i++)
		{
			m_buckets->at(i).Clear();
		}
	}

	void RenderCommandQueue::Sort()
	{
		for (size_t i = 0; i < m_buckets->size(); i++)
		{
			m_buckets->at(i).Sort();
		}
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
		return m_buckets->at((size_t)bucketType);
	}
}