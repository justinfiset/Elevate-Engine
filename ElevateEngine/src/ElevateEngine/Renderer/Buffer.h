#pragma once

#include "ElevateEngine/Core/Data.h"

#include <vector>
#include <iterator>
#include <string>

namespace Elevate
{
	typedef EngineDataType ShaderDataType;

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		unsigned int size;
		unsigned int offset;
		unsigned int Count;
		unsigned int Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), size(GetDataTypeSize(type)), offset(0), Count(GetDataTypeCount(type)), Normalized(normalized) { }
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_elements(elements) 
		{
			CalculateOffsetAndStride();
		}

		BufferLayout(const std::vector<BufferElement>& elements)
			: m_elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

		inline uint32_t GetStride() const { return m_stride; }
	private:
		void CalculateOffsetAndStride()
		{
			unsigned int offset = 0;
			m_stride = 0;
			for (auto& element : m_elements)
			{
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}
	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(void* vertices, uint32_t size)
			: m_data(vertices), m_size(size) { }

		virtual ~VertexBuffer() = default;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		virtual void SetData(void* newData, uint32_t size) = 0;
		virtual void Resize(uint32_t size) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		static VertexBuffer* Create(void* vertices, uint32_t size);

		uint32_t GetSize() { return m_size; }
	protected:
		void SetSize(uint32_t newSize) { m_size = newSize; }
	private:
		uint32_t m_size;
		void* m_data;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(void* vertices, uint32_t count);
	};
}