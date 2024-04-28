#pragma once

#include <string>

namespace Elevate
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSouce);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_RendererID;
	};
}

