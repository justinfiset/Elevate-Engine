add_library("ElevateEngine" STATIC
      "ElevateEngine/src/ElevateEngine/Core/Application.cpp"
      "ElevateEngine/src/ElevateEngine/Core/Application.h"
      "ElevateEngine/src/ElevateEngine/Core/Assert.h"
      "ElevateEngine/src/ElevateEngine/Core/Core.h"
      "ElevateEngine/src/ElevateEngine/Core/Data.h"
      "ElevateEngine/src/ElevateEngine/Core/EntryPoint.h"
      "ElevateEngine/src/ElevateEngine/Core/ITransformable.h"
        "ElevateEngine/src/ElevateEngine/Core/Layers/Layer.cpp"
        "ElevateEngine/src/ElevateEngine/Core/Layers/Layer.h"
        "ElevateEngine/src/ElevateEngine/Core/Layers/LayerStack.cpp"
        "ElevateEngine/src/ElevateEngine/Core/Layers/LayerStack.h"
      "ElevateEngine/src/ElevateEngine/Core/Log.cpp"
      "ElevateEngine/src/ElevateEngine/Core/Log.h"
      "ElevateEngine/src/ElevateEngine/Core/Time.h"
      "ElevateEngine/src/ElevateEngine/Core/Transform.cpp"
      "ElevateEngine/src/ElevateEngine/Core/Transform.h"
      "ElevateEngine/src/ElevateEngine/Core/Window.h"
      "ElevateEngine/src/ElevateEngine/Events/ApplicationEvent.h"
      "ElevateEngine/src/ElevateEngine/Events/Event.h"
      "ElevateEngine/src/ElevateEngine/Events/KeyEvent.h"
      "ElevateEngine/src/ElevateEngine/Events/MouseEvent.h"
      "ElevateEngine/src/ElevateEngine/Files/FileUtility.cpp"
      "ElevateEngine/src/ElevateEngine/Files/FileUtility.h"
      "ElevateEngine/src/ElevateEngine/ImGui/ImGuiLayer.cpp"
      "ElevateEngine/src/ElevateEngine/ImGui/ImGuiLayer.h"
      "ElevateEngine/src/ElevateEngine/ImGui/ImGuiiBuild.cpp"
      "ElevateEngine/src/ElevateEngine/Inputs/Gamepad.h"
      "ElevateEngine/src/ElevateEngine/Inputs/Input.h"
      "ElevateEngine/src/ElevateEngine/Inputs/InputBuffer.cpp"
      "ElevateEngine/src/ElevateEngine/Inputs/InputBuffer.h"
      "ElevateEngine/src/ElevateEngine/Inputs/Joystick.h"
      "ElevateEngine/src/ElevateEngine/Inputs/KeyCodes.h"
      "ElevateEngine/src/ElevateEngine/Inputs/MouseButtonCodes.h"
      "ElevateEngine/src/ElevateEngine/Renderer/Buffer.cpp"
      "ElevateEngine/src/ElevateEngine/Renderer/Buffer.h"
      "ElevateEngine/src/ElevateEngine/Renderer/Camera.cpp"
      "ElevateEngine/src/ElevateEngine/Renderer/Camera.h"
      "ElevateEngine/src/ElevateEngine/Renderer/GraphicsContext.h"
        "ElevateEngine/src/ElevateEngine/Renderer/OpenGL/OpenGLBuffer.cpp"
        "ElevateEngine/src/ElevateEngine/Renderer/OpenGL/OpenGLBuffer.h"
        "ElevateEngine/src/ElevateEngine/Renderer/OpenGL/OpenGLContext.cpp"
        "ElevateEngine/src/ElevateEngine/Renderer/OpenGL/OpenGLContext.h"
        "ElevateEngine/src/ElevateEngine/Renderer/OpenGL/OpenGLVertexArray.cpp"
        "ElevateEngine/src/ElevateEngine/Renderer/OpenGL/OpenGLVertexArray.h"
          "ElevateEngine/src/ElevateEngine/Renderer/OpenGL/Shader/OpenGLShader.cpp"
          "ElevateEngine/src/ElevateEngine/Renderer/OpenGL/Shader/OpenGLShader.h"
          "ElevateEngine/src/ElevateEngine/Renderer/OpenGL/Texture/OpenGLTexture.cpp"
          "ElevateEngine/src/ElevateEngine/Renderer/OpenGL/Texture/OpenGLTexture.h"
      "ElevateEngine/src/ElevateEngine/Renderer/OpenGLRendererAPI.cpp"
      "ElevateEngine/src/ElevateEngine/Renderer/OpenGLRendererAPI.h"
      "ElevateEngine/src/ElevateEngine/Renderer/Renderer.cpp"
      "ElevateEngine/src/ElevateEngine/Renderer/Renderer.h"
      "ElevateEngine/src/ElevateEngine/Renderer/RendererAPI.cpp"
      "ElevateEngine/src/ElevateEngine/Renderer/RendererAPIr.h"
      "ElevateEngine/src/ElevateEngine/Renderer/Shader.cpp"
      "ElevateEngine/src/ElevateEngine/Renderer/Shader.h"
      "ElevateEngine/src/ElevateEngine/Renderer/Texture.cpp"
      "ElevateEngine/src/ElevateEngine/Renderer/Texture.h"
      "ElevateEngine/src/ElevateEngine/Renderer/VertexArray.cpp"
      "ElevateEngine/src/ElevateEngine/Renderer/VertexArray.h"
  "ElevateEngine/src/ElevateEngine.h"
      "ElevateEngine/src/Platform/Windows/WindowsInput.cpp"
      "ElevateEngine/src/Platform/Windows/WindowsInput.h"
      "ElevateEngine/src/Platform/Windows/WindowsWindow.cpp"
      "ElevateEngine/src/Platform/Windows/WindowsWindow.h"
  "ElevateEngine/src/eepch.cpp"
  "ElevateEngine/src/eepch.h"
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_dependencies("ElevateEngine"
    "GLFW"
    "Glad"
    "ImGui"
  )
  set_target_properties("ElevateEngine" PROPERTIES
    OUTPUT_NAME "ElevateEngine"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Debug-windows-x86_64/ElevateEngine"
    LIBRARY_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Debug-windows-x86_64/ElevateEngine"
    RUNTIME_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Debug-windows-x86_64/ElevateEngine"
  )
endif()
target_include_directories("ElevateEngine" PRIVATE
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/src>
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/include>
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/vendor/glm/>
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/vendor/stb/>
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/vendor/Glad/include>
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/vendor/imgui/>
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/vendor/spdlog/include>
)
target_compile_definitions("ElevateEngine" PRIVATE
  $<$<CONFIG:Debug>:_CRT_SECURE_NO_WARNINGS>
  $<$<CONFIG:Debug>:EE_PLATFORM_WINDOWS>
  $<$<CONFIG:Debug>:GLFW_INCLUDE_NONE>
  $<$<CONFIG:Debug>:EE_DEBUG>
)
target_link_directories("ElevateEngine" PRIVATE
)
target_link_libraries("ElevateEngine"
  $<$<CONFIG:Debug>:GLFW>
  $<$<CONFIG:Debug>:Glad>
  $<$<CONFIG:Debug>:ImGui>
  $<$<CONFIG:Debug>:opengl32.lib>
)
target_compile_options("ElevateEngine" PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("ElevateEngine" PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Debug)
target_precompile_headers("ElevateEngine" PUBLIC src/eepch.h)
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
  add_dependencies("ElevateEngine"
    "GLFW"
    "Glad"
    "ImGui"
  )
  set_target_properties("ElevateEngine" PROPERTIES
    OUTPUT_NAME "ElevateEngine"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Release-windows-x86_64/ElevateEngine"
    LIBRARY_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Release-windows-x86_64/ElevateEngine"
    RUNTIME_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Release-windows-x86_64/ElevateEngine"
  )
endif()
target_include_directories("ElevateEngine" PRIVATE
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/src>
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/include>
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/vendor/glm/>
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/vendor/stb/>
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/vendor/Glad/include>
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/vendor/imgui/>
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/vendor/spdlog/include>
)
target_compile_definitions("ElevateEngine" PRIVATE
  $<$<CONFIG:Release>:_CRT_SECURE_NO_WARNINGS>
  $<$<CONFIG:Release>:EE_PLATFORM_WINDOWS>
  $<$<CONFIG:Release>:GLFW_INCLUDE_NONE>
  $<$<CONFIG:Release>:EE_RELEASE>
)
target_link_directories("ElevateEngine" PRIVATE
)
target_link_libraries("ElevateEngine"
  $<$<CONFIG:Release>:GLFW>
  $<$<CONFIG:Release>:Glad>
  $<$<CONFIG:Release>:ImGui>
  $<$<CONFIG:Release>:opengl32.lib>
)
target_compile_options("ElevateEngine" PRIVATE
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("ElevateEngine" PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
target_precompile_headers("ElevateEngine" PUBLIC src/eepch.h)
endif()
if(CMAKE_BUILD_TYPE STREQUAL Dist)
  add_dependencies("ElevateEngine"
    "GLFW"
    "Glad"
    "ImGui"
  )
  set_target_properties("ElevateEngine" PROPERTIES
    OUTPUT_NAME "ElevateEngine"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Dist-windows-x86_64/ElevateEngine"
    LIBRARY_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Dist-windows-x86_64/ElevateEngine"
    RUNTIME_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Dist-windows-x86_64/ElevateEngine"
  )
endif()
target_include_directories("ElevateEngine" PRIVATE
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/src>
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/include>
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/vendor/glm/>
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/vendor/stb/>
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/vendor/Glad/include>
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/vendor/imgui/>
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/vendor/spdlog/include>
)
target_compile_definitions("ElevateEngine" PRIVATE
  $<$<CONFIG:Dist>:_CRT_SECURE_NO_WARNINGS>
  $<$<CONFIG:Dist>:EE_PLATFORM_WINDOWS>
  $<$<CONFIG:Dist>:GLFW_INCLUDE_NONE>
  $<$<CONFIG:Dist>:EE_DIST>
)
target_link_directories("ElevateEngine" PRIVATE
)
target_link_libraries("ElevateEngine"
  $<$<CONFIG:Dist>:GLFW>
  $<$<CONFIG:Dist>:Glad>
  $<$<CONFIG:Dist>:ImGui>
  $<$<CONFIG:Dist>:opengl32.lib>
)
target_compile_options("ElevateEngine" PRIVATE
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:CXX>>:-O2>
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
)
if(CMAKE_BUILD_TYPE STREQUAL Dist)
  set_target_properties("ElevateEngine" PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Dist)
target_precompile_headers("ElevateEngine" PUBLIC src/eepch.h)
endif()