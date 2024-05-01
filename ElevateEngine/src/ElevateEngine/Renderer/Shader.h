#pragma once

#include <string>

namespace Elevate
{
	class Shader
	{
	public:
		~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& vertexSource, const std::string& fragmentSouce);
		static Shader* CreateFromFiles(std::string vertexSrcPath, std::string fragSrcPath);
	};
}

