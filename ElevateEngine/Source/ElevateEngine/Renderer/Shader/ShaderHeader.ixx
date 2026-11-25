module;

#include <string_view>

export module Elevate.Renderer.Shaders:ShaderHeader;

export namespace Elevate::Internal
{
#ifdef EE_PLATFORM_WEB
	constexpr std::string_view ShaderHeader =
		"#version 300 es\n"
		"precision highp float;\n";
#else
	constexpr std::string_view ShaderHeader =
		"#version 410 core\n";
#endif
}
