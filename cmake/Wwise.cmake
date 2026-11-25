set(EE_ENABLE_WWISE "AUTO" CACHE STRING "Wwise integration (AUTO/ON/OFF)")
set_property(CACHE EE_ENABLE_WWISE PROPERTY STRINGS AUTO ON OFF)

set(ee_wwise_enabled FALSE)
if(EE_ENABLE_WWISE STREQUAL "AUTO")
  if(DEFINED ENV{WWISESDK} AND IS_DIRECTORY "$ENV{WWISESDK}")
    set(ee_wwise_enabled TRUE)
  endif()
elseif(EE_ENABLE_WWISE STREQUAL "ON")
  if(NOT DEFINED ENV{WWISESDK} OR NOT IS_DIRECTORY "$ENV{WWISESDK}")
    message(FATAL_ERROR "EE_ENABLE_WWISE=ON requires WWISESDK to point to a valid SDK directory")
  endif()
  set(ee_wwise_enabled TRUE)
elseif(NOT EE_ENABLE_WWISE STREQUAL "OFF")
  message(FATAL_ERROR "Invalid EE_ENABLE_WWISE value: ${EE_ENABLE_WWISE}")
endif()

set(ee_wwise_dest "${CMAKE_SOURCE_DIR}/ElevateEngine/Source/ElevateEngine/Audio/Ak")

if(ee_wwise_enabled)
  set(EE_WWISE_SDK "$ENV{WWISESDK}")
  file(TO_CMAKE_PATH "${EE_WWISE_SDK}" EE_WWISE_SDK)
  message(STATUS "Wwise SDK found at: ${EE_WWISE_SDK}")

  if(WIN32)
    set(ee_wwise_sample_platform "Win32")
    set(ee_wwise_debug_lib "${EE_WWISE_SDK}/x64_vc170/Debug(StaticCRT)/lib")
    set(ee_wwise_debug_bin "${EE_WWISE_SDK}/x64_vc170/Debug(StaticCRT)/bin")
    set(ee_wwise_release_lib "${EE_WWISE_SDK}/x64_vc170/Release(StaticCRT)/lib")
    set(ee_wwise_release_bin "${EE_WWISE_SDK}/x64_vc170/Release(StaticCRT)/bin")
  elseif(EMSCRIPTEN)
    set(ee_wwise_sample_platform "POSIX")
    set(ee_wwise_debug_lib "${EE_WWISE_SDK}/Emscripten_mt/Debug/lib")
    set(ee_wwise_debug_bin "${EE_WWISE_SDK}/Emscripten_mt/Debug/bin")
    set(ee_wwise_release_lib "${EE_WWISE_SDK}/Emscripten_mt/Release/lib")
    set(ee_wwise_release_bin "${EE_WWISE_SDK}/Emscripten_mt/Release/bin")
  else()
    set(ee_wwise_sample_platform "POSIX")
    set(ee_wwise_debug_lib "${EE_WWISE_SDK}/Linux_x64/Debug/lib")
    set(ee_wwise_debug_bin "${EE_WWISE_SDK}/Linux_x64/Debug/bin")
    set(ee_wwise_release_lib "${EE_WWISE_SDK}/Linux_x64/Release/lib")
    set(ee_wwise_release_bin "${EE_WWISE_SDK}/Linux_x64/Release/bin")
  endif()

  file(GLOB_RECURSE wwiseDbSources
    "${EE_WWISE_SDK}/samples/WwiseProjectDatabase/*.cpp"
    "${EE_WWISE_SDK}/samples/WwiseProjectDatabase/*.h"
  )
  # The SDK sample tree also contains Catch2-based test programs. They are not
  # part of the library and would pull in headers we do not provide.
  list(FILTER wwiseDbSources EXCLUDE REGEX "WwiseProjectDatabaseTests|WwiseProjectDatabaseIntegrationTests")
  add_library(WwiseProjectDatabase STATIC ${wwiseDbSources})
  target_include_directories(WwiseProjectDatabase PUBLIC
    "${EE_WWISE_SDK}/samples/WwiseProjectDatabase/WwiseProjectDatabase"
    "${EE_WWISE_SDK}/include"
    "${EE_WWISE_SDK}/samples/3rdParty/subprojects/rapidjson/include"
  )
  target_compile_definitions(WwiseProjectDatabase PUBLIC
    WIN32_LEAN_AND_MEAN
    WWISE_DB_STANDARD_TYPES
    TESTING_WWISE_PROJECT_DATABASE
  )
  if(WIN32)
    # Wwise's AkOSChar is wchar_t on Windows. Select the wide Win32 aliases so
    # the SDK samples compile with the same API set under Ninja and MSBuild.
    target_compile_definitions(WwiseProjectDatabase PRIVATE UNICODE _UNICODE)
  endif()
  target_compile_definitions(WwiseProjectDatabase PUBLIC
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:Editor_Debug>>:_DEBUG>
  )
  target_compile_definitions(WwiseProjectDatabase PUBLIC
    $<$<OR:$<CONFIG:Release>,$<CONFIG:Editor_Release>,$<CONFIG:Dist>>:NDEBUG>
    $<$<OR:$<CONFIG:Release>,$<CONFIG:Editor_Release>,$<CONFIG:Dist>>:AK_OPTIMIZED>
  )
  set_target_properties(WwiseProjectDatabase PROPERTIES
    CXX_STANDARD 20
    POSITION_INDEPENDENT_CODE ON
  )
  ee_setup_outputs(WwiseProjectDatabase)
else()
  message(STATUS "Wwise SDK not found. Skipping Wwise integration.")
endif()

function(ee_wwise_prepare)
  if(NOT ee_wwise_enabled)
    file(REMOVE_RECURSE "${ee_wwise_dest}")
    return()
  endif()

  file(REMOVE_RECURSE "${ee_wwise_dest}")
  file(MAKE_DIRECTORY "${ee_wwise_dest}")
  file(COPY "${EE_WWISE_SDK}/samples/SoundEngine/Common" DESTINATION "${ee_wwise_dest}")
  file(COPY "${EE_WWISE_SDK}/samples/SoundEngine/${ee_wwise_sample_platform}" DESTINATION "${ee_wwise_dest}")
endfunction()

function(ee_wwise_configure_engine target)
  if(NOT ee_wwise_enabled)
    return()
  endif()

  if(WIN32)
    # Wwise's AkOSChar is wchar_t on Windows. The VS generator otherwise uses
    # the ANSI Win32 aliases, which do not accept AkOSChar paths.
    target_compile_definitions(${target} PUBLIC UNICODE _UNICODE)
  endif()

  target_compile_definitions(${target} PUBLIC
    EE_USES_WWISE
    EE_USES_WWISEPROJECTDATABASE
  )
  target_compile_definitions(${target} PUBLIC
    $<$<OR:$<CONFIG:Release>,$<CONFIG:Editor_Release>,$<CONFIG:Dist>>:AK_OPTIMIZED>
  )

  target_include_directories(${target} PUBLIC
    "${EE_WWISE_SDK}/include"
    "${ee_wwise_dest}/${ee_wwise_sample_platform}"
    "${EE_WWISE_SDK}/samples"
    "${EE_WWISE_SDK}/samples/3rdParty/subprojects"
    "${EE_WWISE_SDK}/samples/WwiseProjectDatabase/WwiseProjectDatabase"
    "${EE_WWISE_SDK}/samples/SoundEngine/Common"
    "${EE_WWISE_SDK}/samples/SoundEngine/${ee_wwise_sample_platform}"
  )

  foreach(cfg Debug Editor_Debug)
    target_link_directories(${target} PUBLIC
      "$<$<CONFIG:${cfg}>:${ee_wwise_debug_lib}>"
      "$<$<CONFIG:${cfg}>:${ee_wwise_debug_bin}>"
    )
  endforeach()
  foreach(cfg Release Editor_Release Dist)
    target_link_directories(${target} PUBLIC
      "$<$<CONFIG:${cfg}>:${ee_wwise_release_lib}>"
      "$<$<CONFIG:${cfg}>:${ee_wwise_release_bin}>"
    )
  endforeach()

  target_link_libraries(${target} PUBLIC
    WwiseProjectDatabase
    AkSoundEngine
    AkMemoryMgr
    AkStreamMgr
    AkSpatialAudio
    AkVorbisDecoder
    AkOpusDecoder
  )
  target_link_libraries(${target} PUBLIC
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:Editor_Debug>>:CommunicationCentral>
  )

  if(WIN32)
    target_link_libraries(${target} PUBLIC
      $<$<OR:$<CONFIG:Editor_Debug>,$<CONFIG:Editor_Release>>:AkAutobahn>
      $<$<OR:$<CONFIG:Editor_Debug>,$<CONFIG:Editor_Release>>:ws2_32>
    )
    target_link_options(${target} INTERFACE /IGNORE:4006)
  endif()
endfunction()
