set(ELEVATE_CONFIGS Editor_Debug Editor_Release Debug Release Dist)

set(ELEVATE_ROOT "${CMAKE_SOURCE_DIR}")
set(ELEVATE_VENDOR "${CMAKE_SOURCE_DIR}/ElevateEngine/Vendor")
set(ELEVATE_ENGINE_SOURCE "${CMAKE_SOURCE_DIR}/ElevateEngine/Source")

if(EMSCRIPTEN)
  set(EE_PLATFORM_TAG "emscripten")
  set(EE_ARCH_TAG "wasm32")
  set(EE_PLATFORM_DEFINE "EE_PLATFORM_WEB")
  set(EE_RESOURCE_DIR_VALUE "/")
elseif(WIN32)
  set(EE_PLATFORM_TAG "windows")
  set(EE_ARCH_TAG "x86_64")
  set(EE_PLATFORM_DEFINE "EE_PLATFORM_WINDOWS")
  set(EE_RESOURCE_DIR_VALUE "${CMAKE_SOURCE_DIR}/ElevateEngine/Resources/")
else()
  set(EE_PLATFORM_TAG "linux")
  set(EE_ARCH_TAG "x86_64")
  set(EE_PLATFORM_DEFINE "EE_PLATFORM_LINUX;STBI_NO_SIMD")
  set(EE_RESOURCE_DIR_VALUE "${CMAKE_SOURCE_DIR}/ElevateEngine/Resources/")
endif()

if(NOT EMSCRIPTEN)
  file(TO_CMAKE_PATH "${EE_RESOURCE_DIR_VALUE}" EE_RESOURCE_DIR_VALUE)
endif()

# Output layout used by CI and the web deployment job:
#   Build/bin/<Config>-<platform>-<arch>/<target>
function(ee_setup_outputs target)
  foreach(cfg IN LISTS ELEVATE_CONFIGS)
    string(TOUPPER "${cfg}" cfg_upper)
    set(base "${CMAKE_SOURCE_DIR}/Build/bin/${cfg}-${EE_PLATFORM_TAG}-${EE_ARCH_TAG}/${target}")
    set_target_properties("${target}" PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} "${base}"
      ARCHIVE_OUTPUT_DIRECTORY_${cfg_upper} "${base}"
      LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} "${base}"
      PDB_OUTPUT_DIRECTORY_${cfg_upper} "${base}"
    )
  endforeach()
endfunction()

function(ee_web_compile_settings target)
  # All objects linked into a pthreaded Wasm binary must agree on atomics,
  # bulk-memory and SIMD target features, including vendored static libraries.
  target_compile_options(${target} PRIVATE
    -pthread -matomics -mbulk-memory -msimd128 -msse4.1
  )
endfunction()

function(ee_common_settings target)
  target_compile_features(${target} PUBLIC cxx_std_20)
  target_compile_definitions(${target} PUBLIC
    ${EE_PLATFORM_DEFINE}
    GLM_ENABLE_EXPERIMENTAL
    "EE_RESOURCE_DIR=\"${EE_RESOURCE_DIR_VALUE}\""
  )

  target_compile_definitions(${target} PUBLIC
    $<$<CONFIG:Editor_Debug>:EE_DEBUG>
    $<$<CONFIG:Editor_Debug>:EE_EDITOR_BUILD>
    $<$<CONFIG:Editor_Debug>:JPH_ENABLE_ASSERTS>
    $<$<CONFIG:Editor_Release>:EE_RELEASE>
    $<$<CONFIG:Editor_Release>:EE_EDITOR_BUILD>
    $<$<CONFIG:Editor_Release>:NDEBUG>
    $<$<CONFIG:Debug>:EE_DEBUG>
    $<$<CONFIG:Debug>:JPH_ENABLE_ASSERTS>
    $<$<CONFIG:Release>:EE_RELEASE>
    $<$<CONFIG:Release>:NDEBUG>
    $<$<CONFIG:Dist>:EE_DIST>
    $<$<CONFIG:Dist>:NDEBUG>
  )

  if(MSVC)
    target_compile_options(${target} PRIVATE /Zc:wchar_t /Zc:preprocessor /Zc:__cplusplus /utf-8 /MP)
    target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
  elseif(EMSCRIPTEN)
    ee_web_compile_settings(${target})
    target_compile_options(${target} PRIVATE
      -Wno-deprecated-literal-operator -Wno-invalid-offsetof -Wno-deprecated-pragma
    )
  else()
    target_compile_options(${target} PRIVATE -fno-lto)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      # GCC cannot stream OPTIMIZATION_NODEs into a module BMI at -O0, and
      # bundled fmt emits #pragma GCC optimize("Og") in debug builds (PR 108080).
      # -Og keeps full debug info while avoiding the ICE.
      target_compile_options(${target} PRIVATE
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<OR:$<CONFIG:Editor_Debug>,$<CONFIG:Debug>>>:-Og>
      )
      # GCC can exhaust location_t space while streaming module BMIs that
      # transitively include fmt/glm headers (PR 105443).  Disabling macro
      # expansion tracking keeps the location maps smaller; GCC 15 contains
      # the write_location fix, and diagnostics lose macro detail only.
      target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-ftrack-macro-expansion=0>
      )
      # GCC's IPA comdat pass ICEs when a declaration imported from a module
      # appears late in the symbol table (ipa_comdats.cc:355).  This is an
      # optimization-only pass; disabling it affects symbol section grouping,
      # not program behavior.
      target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-fdisable-ipa-comdats>
      )
      # GCC's lazy BMI loading recurses on this project's template graph
      # ("recursive lazy load" / "failed to load pendings").  Eager loading is
      # the only reliable workaround; drop this once the compiler is fixed.
      target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-fno-module-lazy>
      )
    endif()
  endif()

  if(WIN32)
    target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
  endif()

  # Root-level scanning is OFF so vendored code is untouched; module-aware
  # targets opt in here. FILE_SET CXX_MODULES sources are always scanned.
  set_target_properties(${target} PROPERTIES CXX_SCAN_FOR_MODULES ON)
  ee_setup_outputs(${target})
endfunction()

function(ee_safe_project_name name output)
  string(REPLACE " " "-" safe "${name}")
  set(${output} "${safe}" PARENT_SCOPE)
endfunction()
