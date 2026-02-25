#pragma once

namespace Elevate
{
	// TODO : CREATE A BUILDER PATTERN
    struct RenderState
	{
		bool Cullface = true;
		bool DepthWrite = true; // Should we write in the depth buffer?
		bool DepthTest = true; // Should we test the depth?
	};
}