add_library("GLFW" STATIC
      "ElevateEngine/vendor/GLFW/include/GLFW/glfw3.h"
      "ElevateEngine/vendor/GLFW/include/GLFW/glfw3native.h"
    "ElevateEngine/vendor/GLFW/src/context.c"
    "ElevateEngine/vendor/GLFW/src/egl_context.c"
    "ElevateEngine/vendor/GLFW/src/glfw_config.h"
    "ElevateEngine/vendor/GLFW/src/init.c"
    "ElevateEngine/vendor/GLFW/src/input.c"
    "ElevateEngine/vendor/GLFW/src/monitor.c"
    "ElevateEngine/vendor/GLFW/src/osmesa_context.c"
    "ElevateEngine/vendor/GLFW/src/vulkan.c"
    "ElevateEngine/vendor/GLFW/src/wgl_context.c"
    "ElevateEngine/vendor/GLFW/src/win32_init.c"
    "ElevateEngine/vendor/GLFW/src/win32_joystick.c"
    "ElevateEngine/vendor/GLFW/src/win32_monitor.c"
    "ElevateEngine/vendor/GLFW/src/win32_thread.c"
    "ElevateEngine/vendor/GLFW/src/win32_time.c"
    "ElevateEngine/vendor/GLFW/src/win32_window.c"
    "ElevateEngine/vendor/GLFW/src/window.c"
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("GLFW" PROPERTIES
    OUTPUT_NAME "GLFW"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/bin/Debug-windows-x86_64/GLFW"
    LIBRARY_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/bin/Debug-windows-x86_64/GLFW"
    RUNTIME_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/bin/Debug-windows-x86_64/GLFW"
  )
endif()
target_include_directories("GLFW" PRIVATE
)
target_compile_definitions("GLFW" PRIVATE
  $<$<CONFIG:Debug>:_GLFW_WIN32>
  $<$<CONFIG:Debug>:_CRT_SECURE_NO_WARNINGS>
)
target_link_directories("GLFW" PRIVATE
)
target_link_libraries("GLFW"
)
target_compile_options("GLFW" PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("GLFW" PROPERTIES
    OUTPUT_NAME "GLFW"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/bin/Release-windows-x86_64/GLFW"
    LIBRARY_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/bin/Release-windows-x86_64/GLFW"
    RUNTIME_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/bin/Release-windows-x86_64/GLFW"
  )
endif()
target_include_directories("GLFW" PRIVATE
)
target_compile_definitions("GLFW" PRIVATE
  $<$<CONFIG:Release>:_GLFW_WIN32>
  $<$<CONFIG:Release>:_CRT_SECURE_NO_WARNINGS>
)
target_link_directories("GLFW" PRIVATE
)
target_link_libraries("GLFW"
)
target_compile_options("GLFW" PRIVATE
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
)
if(CMAKE_BUILD_TYPE STREQUAL Dist)
  set_target_properties("GLFW" PROPERTIES
    OUTPUT_NAME "GLFW"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/bin/Dist-windows-x86_64/GLFW"
    LIBRARY_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/bin/Dist-windows-x86_64/GLFW"
    RUNTIME_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/ElevateEngine/vendor/GLFW/bin/Dist-windows-x86_64/GLFW"
  )
endif()
target_include_directories("GLFW" PRIVATE
)
target_compile_definitions("GLFW" PRIVATE
  $<$<CONFIG:Dist>:_GLFW_WIN32>
  $<$<CONFIG:Dist>:_CRT_SECURE_NO_WARNINGS>
)
target_link_directories("GLFW" PRIVATE
)
target_link_libraries("GLFW"
)
target_compile_options("GLFW" PRIVATE
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:CXX>>:-m64>
)