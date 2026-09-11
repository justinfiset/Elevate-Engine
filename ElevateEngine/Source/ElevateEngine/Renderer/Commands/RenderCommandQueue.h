#pragma once

#include <array>

#include "RenderBucket.h"
#include "RenderCommand.h"
#include <ElevateEngine/Renderer/VertexArray.h>

namespace Elevate
{
	class Material;

	class RenderCommandQueue
	{
	public:
		void Submit(RenderBucket::Type type, const RenderCommand& command);
		void Clear();
		void Flush(RenderBucket::Type type);
		void FlushAll();
		uint64_t GetCount();
		const RenderBucket& GetBucket(RenderBucket::Type bucketType) const;

	protected:
		std::array<RenderBucket, static_cast<size_t>(RenderBucket::Type::Count)> m_buckets;
		uint64_t m_count = 0;
	};
}