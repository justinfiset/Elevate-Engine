#pragma once

namespace Elevate
{
	class VertexArray
	{
	public:
		~VertexArray() { }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void LinkAttribute(int32_t layout, uint32_t numComponent, uint32_t type, uint32_t stride, void* offset) const = 0;

		static VertexArray* Create();
	};
}