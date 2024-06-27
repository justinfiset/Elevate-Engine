add_executable("Sandbox"
  "Sandbox/src/SandboxApp.cpp"
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_dependencies("Sandbox"
    "ElevateEngine"
  )
  set_target_properties("Sandbox" PROPERTIES
    OUTPUT_NAME "Sandbox"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Debug-windows-x86_64/Sandbox"
    LIBRARY_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Debug-windows-x86_64/Sandbox"
    RUNTIME_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Debug-windows-x86_64/Sandbox"
  )
endif()
target_include_directories("Sandbox" PRIVATE
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/Sandbox/vendor/include>
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/vendor/glm/>
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/vendor/stb/>
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/vendor>
  $<$<CONFIG:Debug>:C:/Git/Elevate-Engine/ElevateEngine/src>
)
target_compile_definitions("Sandbox" PRIVATE
  $<$<CONFIG:Debug>:EE_PLATFORM_WINDOWS>
  $<$<CONFIG:Debug>:EE_DEBUG>
)
target_link_directories("Sandbox" PRIVATE
)
target_link_libraries("Sandbox"
  $<$<CONFIG:Debug>:ElevateEngine>
)
target_compile_options("Sandbox" PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("Sandbox" PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
  add_dependencies("Sandbox"
    "ElevateEngine"
  )
  set_target_properties("Sandbox" PROPERTIES
    OUTPUT_NAME "Sandbox"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Release-windows-x86_64/Sandbox"
    LIBRARY_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Release-windows-x86_64/Sandbox"
    RUNTIME_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Release-windows-x86_64/Sandbox"
  )
endif()
target_include_directories("Sandbox" PRIVATE
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/Sandbox/vendor/include>
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/vendor/glm/>
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/vendor/stb/>
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/vendor>
  $<$<CONFIG:Release>:C:/Git/Elevate-Engine/ElevateEngine/src>
)
target_compile_definitions("Sandbox" PRIVATE
  $<$<CONFIG:Release>:EE_PLATFORM_WINDOWS>
  $<$<CONFIG:Release>:EE_RELEASE>
)
target_link_directories("Sandbox" PRIVATE
)
target_link_libraries("Sandbox"
  $<$<CONFIG:Release>:ElevateEngine>
)
target_compile_options("Sandbox" PRIVATE
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("Sandbox" PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Dist)
  add_dependencies("Sandbox"
    "ElevateEngine"
  )
  set_target_properties("Sandbox" PROPERTIES
    OUTPUT_NAME "Sandbox"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Dist-windows-x86_64/Sandbox"
    LIBRARY_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Dist-windows-x86_64/Sandbox"
    RUNTIME_OUTPUT_DIRECTORY "C:/Git/Elevate-Engine/bin/Dist-windows-x86_64/Sandbox"
  )
endif()
target_include_directories("Sandbox" PRIVATE
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/Sandbox/vendor/include>
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/vendor/glm/>
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/vendor/stb/>
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/vendor>
  $<$<CONFIG:Dist>:C:/Git/Elevate-Engine/ElevateEngine/src>
)
target_compile_definitions("Sandbox" PRIVATE
  $<$<CONFIG:Dist>:EE_PLATFORM_WINDOWS>
  $<$<CONFIG:Dist>:EE_DIST>
)
target_link_directories("Sandbox" PRIVATE
)
target_link_libraries("Sandbox"
  $<$<CONFIG:Dist>:ElevateEngine>
)
target_compile_options("Sandbox" PRIVATE
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:CXX>>:-O2>
  $<$<AND:$<CONFIG:Dist>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
)
if(CMAKE_BUILD_TYPE STREQUAL Dist)
  set_target_properties("Sandbox" PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()