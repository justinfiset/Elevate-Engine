#pragma once

import Elevate.Foundations;

#if defined(EE_DEBUG) && !defined(EE_PLATFORM_WEB)
#define GLCheck(x) \
		x; \
		{ GLenum err = glGetError(); \
		  if (err != GL_NO_ERROR) CoreLogger::Error("OpenGL Error {} at {}:{}", err, __FILE__, __LINE__); }
#else
// glGetError forces a GPU sync on Emscripten. Running it after every GL call
// drops the Web build to ~30 FPS and stalls texture uploads/mipmap generation.
// Native debug builds keep the checks.
#define GLCheck(x) x
#endif
