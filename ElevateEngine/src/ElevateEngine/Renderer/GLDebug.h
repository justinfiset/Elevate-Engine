#pragma once

#include <ElevateEngine/Core/Log.h>

#ifdef EE_DEBUG
#define GLCheck(x) \
        x; \
        { GLenum err = glGetError(); \
          if (err != GL_NO_ERROR) EE_CORE_ERROR("OpenGL Error {} at {}:{}", err, __FILE__, __LINE__); }
#else
#define GLCheck(x) x
#endif