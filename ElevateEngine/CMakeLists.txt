file(GLOB_RECURSE engineModules CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.ixx")
file(GLOB_RECURSE engineSources CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.inl"
)

file(GLOB vendoredSources
  "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/ImGuizmo/src/ImGuizmo.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/ImGuiFileDialog/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/tinyfiledialogs/tinyfiledialogs.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/spdlog/src/*.cpp"
)
if(NOT EMSCRIPTEN)
  list(APPEND vendoredSources "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/Glad/src/glad.c")
endif()

set_source_files_properties(${engineModules} PROPERTIES LANGUAGE CXX)

ee_source_group_tree("${CMAKE_CURRENT_SOURCE_DIR}/Source" "Source" ${engineModules} ${engineSources})
ee_source_group_tree("${CMAKE_CURRENT_SOURCE_DIR}/Vendor" "Vendor" ${vendoredSources})

add_library(ElevateEngine STATIC ${engineSources} ${vendoredSources})
target_sources(ElevateEngine PUBLIC
  FILE_SET CXX_MODULES
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Source"
  FILES ${engineModules}
)

ee_common_settings(ElevateEngine)

target_compile_definitions(ElevateEngine PUBLIC
  EE_ENGINE_BUILD
  SPDLOG_COMPILED_LIB
  IMGUI_DEFINE_MATH_OPERATORS
  GLM_ENABLE_EXPERIMENTAL
)

target_include_directories(ElevateEngine PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/Source"
  "${CMAKE_CURRENT_SOURCE_DIR}/Vendor"
  "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/ImGui"
  "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glm"
  "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/spdlog/include"
)

target_link_libraries(ElevateEngine PUBLIC ElevateVendorHeaders ImGui assimp Jolt)

if(NOT EMSCRIPTEN)
  target_link_libraries(ElevateEngine PUBLIC GLFW)
  if(WIN32)
    target_link_libraries(ElevateEngine PUBLIC opengl32)
  else()
    target_link_libraries(ElevateEngine PUBLIC GL X11 pthread ${CMAKE_DL_LIBS})
  endif()
endif()
