#pragma once

namespace Elevate
{
	class FrameBuffer
	{
	public:
		~FrameBuffer() { }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Rescale(uint32_t width, uint32_t height) const = 0;

		virtual uint32_t GetTextureId() const = 0;
		virtual uint32_t GetFrameBufferId() const = 0;

		static FrameBuffer* Create(uint32_t width, uint32_t height);
	};	
}

