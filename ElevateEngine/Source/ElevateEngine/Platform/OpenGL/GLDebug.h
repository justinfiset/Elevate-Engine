#pragma once

import Elevate.Foundations;

#ifdef EE_DEBUG
#define GLCheck(x) \
		x; \
		{ GLenum err = glGetError(); \
		  if (err != GL_NO_ERROR) CoreLogger::Error("OpenGL Error {} at {}:{}", err, __FILE__, __LINE__); }
#else
#define GLCheck(x) x
#endif