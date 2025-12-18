#pragma once

#include "RenderBucket.h"
#include "RenderCommand.h"
#include <ElevateEngine/Renderer/VertexArray.h>

namespace Elevate
{
	class Material;

	class RenderCommandQueue
	{
	public:
		void Submit(RenderBucket::Type bucketType, std::shared_ptr<VertexArray>& array,/* std::shared_ptr<Material>& material,*/ const glm::mat4& transform, const RenderState& state);
		void Clear();
		void Sort();
		void FlushAll();
		const RenderBucket& GetBucket(RenderBucket::Type bucketType) const;

	protected:
		std::vector<RenderBucket> m_buckets[static_cast<size_t>(RenderBucket::Type::Count)];
	};
}