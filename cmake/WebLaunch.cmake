# The Visual Studio generator cannot use the Emscripten toolchain directly.
# These targets are solution-level shortcuts that drive the existing Ninja
# `web` preset, then serve the resulting project with emrun. They are grouped
# under the owning project's solution folder.

set(EE_WEB_CONFIG "Editor_Debug" CACHE STRING "Fallback configuration for WebRun targets when no multi-config value is available")
set_property(CACHE EE_WEB_CONFIG PROPERTY STRINGS ${ELEVATE_CONFIGS})
set(EE_EMSDK_ROOT "$ENV{EMSDK}" CACHE PATH "Fallback Emscripten SDK root for the Visual Studio WebRun targets")
set(EE_NINJA_EXECUTABLE "" CACHE FILEPATH "Ninja executable used by the Visual Studio WebRun targets")
if(NOT EE_NINJA_EXECUTABLE)
  find_program(eeNinjaDefault NAMES ninja ninja.exe)
  if(eeNinjaDefault)
    set(EE_NINJA_EXECUTABLE "${eeNinjaDefault}" CACHE FILEPATH "Ninja executable used by the Visual Studio WebRun targets" FORCE)
  endif()
endif()

function(ee_add_web_launch_target target projectName projectFolder)
  if(NOT WIN32 OR NOT CMAKE_GENERATOR MATCHES "Visual Studio")
    return()
  endif()

  set(webPlatformTag "emscripten")
  set(webArchTag "wasm32")

  file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}" sourceNative)
  file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/Build/bin" outputRootNative)
  file(TO_NATIVE_PATH "${EE_EMSDK_ROOT}" emsdkRootNative)
  file(TO_NATIVE_PATH "${CMAKE_COMMAND}" cmakeNative)

  set(ninjaNative "")
  set(ninjaDirNative "")
  if(EE_NINJA_EXECUTABLE)
    file(TO_NATIVE_PATH "${EE_NINJA_EXECUTABLE}" ninjaNative)
    get_filename_component(ninjaDir "${ninjaNative}" DIRECTORY)
    file(TO_NATIVE_PATH "${ninjaDir}" ninjaDirNative)
  endif()

  set(EE_SOURCE_DIR "${sourceNative}")
  set(EE_WEB_TARGET "${target}")
  set(EE_WEB_OUTPUT_ROOT "${outputRootNative}")
  set(EE_WEB_OUTPUT_TAGS "${webPlatformTag}-${webArchTag}")
  set(EE_EMSDK_ROOT "${emsdkRootNative}")
  set(EE_CMAKE_EXECUTABLE "${cmakeNative}")
  set(EE_NINJA_EXECUTABLE "${ninjaNative}")
  set(EE_NINJA_DIR "${ninjaDirNative}")

  set(webScript "${CMAKE_BINARY_DIR}/WebLaunch_${target}.bat")
  configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/WebLaunch.bat.in"
    "${webScript}"
    @ONLY
  )

  add_custom_target(${target}_WebRun
    COMMAND "${webScript}" "$<CONFIG>"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Building ${projectName} for Web and starting emrun"
    VERBATIM
  )
  ee_target_folder(${target}_WebRun "${projectFolder}/Web")
endfunction()
