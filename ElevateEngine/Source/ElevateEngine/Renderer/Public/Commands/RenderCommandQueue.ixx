module;

#include <array>

export module Elevate.Renderer.Commands.RenderCommandQueue;

import Elevate.Renderer.Materials.Material;
import Elevate.Renderer.Commands.RenderBucket;
import Elevate.Renderer.Commands.RenderCommand;

export namespace Elevate
{
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