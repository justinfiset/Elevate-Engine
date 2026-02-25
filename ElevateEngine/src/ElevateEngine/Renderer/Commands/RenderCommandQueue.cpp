#include "RenderCommandQueue.h"

namespace Elevate
{
	void RenderCommandQueue::Submit(RenderBucket::Type type, const RenderCommand& command)
	{
		m_buckets.at((size_t)type).Submit(command);
	}

	void RenderCommandQueue::Clear()
	{
		for (size_t i = 0; i < m_buckets.size(); i++)
		{
			m_buckets.at(i).Clear();
		}
	}

	void RenderCommandQueue::Sort()
	{
		for (size_t i = 0; i < m_buckets.size(); i++)
		{
			m_buckets.at(i).Sort();
		}
	}

	void RenderCommandQueue::FlushAll()
	{
		Sort();

		for (size_t i = 0; i < m_buckets.size(); i++)
		{
			m_buckets.at(i).Flush();
		}
		Clear();
	}

	const RenderBucket& RenderCommandQueue::GetBucket(RenderBucket::Type bucketType) const
	{
		return m_buckets.at((size_t)bucketType);
	}
}