#pragma once

#include "glm/glm.hpp"

namespace Elevate
{
	class FrameBuffer
	{
	public:
		~FrameBuffer() { }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Clear() const = 0;
		virtual void Rescale(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetTextureId() const = 0;
		virtual uint32_t GetFrameBufferId() const = 0;

		inline void SetClearColor(glm::vec4 color) { m_ClearColor = color; }

		static FrameBuffer* Create(uint32_t width, uint32_t height);

	protected:
		glm::vec4 m_ClearColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
}

