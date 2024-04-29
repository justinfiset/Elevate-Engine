#pragma once

namespace Elevate
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1,
	};
	
	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
		inline static void SetAPI(RendererAPI API) {  s_RendererAPI = API; }
	private:
		static RendererAPI s_RendererAPI;
	};
}