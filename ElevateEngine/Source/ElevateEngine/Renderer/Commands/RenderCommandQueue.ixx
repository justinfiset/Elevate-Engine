module;

#include <array>

export module Elevate.Renderer.Commands.RenderCommandQueue;

import Elevate.Renderer.Materials.Material;
import Elevate.Renderer.Commands.RenderBucket;
import Elevate.Renderer.Commands.RenderCommand;
import Elevate.Renderer.Commands.RenderBucketType;

export namespace Elevate
{
	class RenderCommandQueue
	{
	public:
		void Submit(RenderBucketType type, const RenderCommand &command);
		void Clear();

		void Flush(RenderBucketType type);
		void FlushAll();

		uint64_t GetCount();
		const RenderBucket &GetBucket(RenderBucketType bucketType) const;

	protected:
		std::array<RenderBucket, static_cast<size_t>(RenderBucketType::Count)> m_buckets;
		uint64_t m_count = 0;
	};
}
