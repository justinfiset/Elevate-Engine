#pragma once

namespace Elevate
{
	class FrameBuffer
	{
	public:
		~FrameBuffer() { }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Rescale(float width, float height) const = 0;
		virtual uint32_t GetTextureID() const = 0;

		static FrameBuffer* Create(int width, int height);
	};	
}

