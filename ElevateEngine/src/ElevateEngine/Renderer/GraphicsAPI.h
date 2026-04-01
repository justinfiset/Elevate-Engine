#pragma once

#if defined(EE_PLATFORM_WEB)
    #include <GLES3/gl3.h>
    #define EE_GLES

    // Translation for modern OpenGL features that are not available in GLES3
    #define GL_CLAMP_TO_BORDER          GL_CLAMP_TO_EDGE
    #define GL_PATCHES                  0x000E
    #define GL_TEXTURE_SWIZZLE_RGBA     0x8E46
#else
    // Safety to prevent double definition and supress warnings
    #ifdef APIENTRY
        #undef APIENTRY
    #endif
    
    #include <glad/glad.h>
    #define EE_SUPPORTS_DSA
#endif