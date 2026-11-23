cmake_minimum_required(VERSION 3.28)

if(POLICY CMP0155)
  cmake_policy(SET CMP0155 NEW)
endif()

project(ElevateEngine VERSION 1.0 LANGUAGES C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if(WIN32)
  set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT Sandbox)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Vendored libraries are not module-aware. Scanning is enabled explicitly on
# the engine, the tests and the user projects (see cmake/ElevateConfig.cmake).
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<OR:$<CONFIG:Debug>,$<CONFIG:Editor_Debug>>:Debug>")
set(CMAKE_CONFIGURATION_TYPES "Editor_Debug;Editor_Release;Debug;Release;Dist" CACHE STRING "Elevate configurations" FORCE)

set(CMAKE_C_FLAGS_EDITOR_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_EDITOR_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_EDITOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_EDITOR_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_EDITOR_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_EDITOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_DIST "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DIST "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# Catch2 and the vendored libraries are configured by their own CMake projects,
# so target-local flags cannot keep their standard library in sync with the
# engine. Set libc++ globally for native Clang builds; Emscripten already uses
# libc++ and does not accept -stdlib.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT EMSCRIPTEN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ElevateConfig)
include(ResourceGen)
include(Dependencies)
include(ProjectHelpers)
include(Wwise)

# Module sources must exist when CMake generates the build graph.
ee_generate_resources(
  "${CMAKE_CURRENT_SOURCE_DIR}/ElevateEngine/Resources/Editor"
  "editor://"
  "${CMAKE_CURRENT_SOURCE_DIR}/ElevateEngine/Source/ElevateEngine/Editor/Generated/EditorResources_generated.ixx"
  "Elevate::Editor::Resources"
  "Elevate.Editor.Resources"
)
ee_generate_resources(
  "${CMAKE_CURRENT_SOURCE_DIR}/ElevateEngine/Resources/Engine"
  "engine://"
  "${CMAKE_CURRENT_SOURCE_DIR}/ElevateEngine/Source/ElevateEngine/Core/Generated/EngineResources_generated.ixx"
  "Elevate::Resources"
  "Elevate.Resources"
)

add_custom_target(resourcegen
  COMMAND "${CMAKE_COMMAND}" -S "${CMAKE_CURRENT_SOURCE_DIR}" -B "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Regenerating embedded resource modules"
)

ee_wwise_prepare()

add_subdirectory(ElevateEngine)
ee_wwise_configure_engine(ElevateEngine)

if(NOT EMSCRIPTEN)
  enable_testing()
  add_subdirectory(ElevateTests)
endif()

include(ProjectDiscovery)
