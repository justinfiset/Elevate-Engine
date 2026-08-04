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
			Transparent,
			UI,
			Debug,
			Count // Only used to keep the number of bucket at compile time
		};

		void Submit(const RenderCommand& command);
		void Sort();
		void Flush() const;
		void Clear();

		size_t GetCommandCount() const;
		size_t GetMemoryUsage() const;

		std::vector<RenderCommand>::iterator begin() { return m_commands.begin(); }
		std::vector<RenderCommand>::iterator end() { return m_commands.end(); }

		std::vector<RenderCommand>::const_iterator begin() const { return m_commands.begin(); }
		std::vector<RenderCommand>::const_iterator end()   const { return m_commands.end(); }

	protected:
		std::vector<RenderCommand> m_commands;
		bool m_isSorted = true;
	};
}