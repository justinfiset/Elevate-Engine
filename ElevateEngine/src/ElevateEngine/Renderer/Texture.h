#pragma once

#include <string>

namespace Elevate
{
	class Texture
	{
	public:
		~Texture() { };
		virtual void Bind(int index = 0) const = 0;

		static Texture* Create(std::string path);
	};
}