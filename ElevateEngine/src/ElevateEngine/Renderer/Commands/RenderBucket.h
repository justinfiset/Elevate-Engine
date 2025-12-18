#pragma once

#include <vector>
#include "RenderCommand.h"

namespace Elevate
{
	class RenderBucket
	{
	public:
		enum Type : uint8_t
		{
			ShadowMap,
			GBuffer,
			Opaque,
			Transparent,
			UI,
			Debug,
			Count // Only used to keep the number of bucket at compile time
		};

		void Submit(const RenderCommand& command);
		void Submit(RenderBucket::Type bucketType, std::shared_ptr<VertexArray>& array, const glm::mat4& transform, const RenderState& state)
		void Sort();
		void Flush();
		void Clear();

		size_t GetCommandCount() const;
		size_t GetMemoryUsage() const;

	protected:
		std::vector<RenderCommand> m_commands;
		bool m_isSorted = true;
	};
}