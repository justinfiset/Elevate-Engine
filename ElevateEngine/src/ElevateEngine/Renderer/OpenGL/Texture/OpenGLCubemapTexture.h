#pragma once

#include <string>
#include "ElevateEngine/Renderer/CubemapTexture.h"

namespace Elevate
{
	class OpenGLCubemapTexture : public CubemapTexture
	{
	public:
		OpenGLCubemapTexture(std::string path);

		virtual void Bind(int index = 0) const override;
		virtual void Unbind(int index = 0) const override;
	};
}