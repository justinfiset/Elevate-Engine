#include "eepch.h"
#include "FrameBuffer.h"

#include "OpenGL/OpenGLFrameBuffer.h"

namespace Elevate
{
	FrameBuffer* FrameBuffer::Create(int width, int height)
	{
		return new OpenGLFrameBuffer(width, height);
	}
}
