#include "eepch.h"
#include "FrameBuffer.h"

#include "OpenGL/OpenGLFrameBuffer.h"

namespace Elevate
{
	FrameBuffer* FrameBuffer::Create(uint32_t width, uint32_t height)
	{
		return new OpenGLFrameBuffer(width, height);
	}
}
