#include "RenderCommandQueue.h"

namespace Elevate
{
	void RenderCommandQueue::Submit(RenderBucket::Type type, const RenderCommand& command)
	{
		m_buckets.at((size_t)type).Submit(command);
		m_count++;
	}

	void RenderCommandQueue::Clear()
	{
		for (size_t i = 0; i < m_buckets.size(); i++)
		{
			m_buckets.at(i).Clear();
		}
		m_count = 0;
	}

	void RenderCommandQueue::FlushAll()
	{
		for (size_t i = 0; i < m_buckets.size(); i++)
		{
			m_buckets.at(i).Sort();
			m_buckets.at(i).Flush();
		}
	}

	uint64_t RenderCommandQueue::GetCount()
	{
		return m_count;
	}

	const RenderBucket& RenderCommandQueue::GetBucket(RenderBucket::Type bucketType) const
	{
		return m_buckets.at((size_t)bucketType);
	}
}