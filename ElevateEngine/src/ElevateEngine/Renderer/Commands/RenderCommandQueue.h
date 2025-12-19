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
		void Sort();
		void FlushAll();
		const RenderBucket& GetBucket(RenderBucket::Type bucketType) const;

	protected:
		std::array<RenderBucket, static_cast<size_t>(RenderBucket::Type::Count)> m_buckets;
	};
}