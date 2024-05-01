#pragma once

#include "ElevateEngine/Renderer/VertexArray.h"

namespace Elevate
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();

		virtual void Bind() const;
		virtual void Unbind() const;
		virtual void LinkAttribute(int32_t layout, uint32_t numComponent, uint32_t type, uint32_t stride, void* offset) const;

	private:
		unsigned int m_RendererID;
	};
}