function(ee_parse_eeproj projectDir nameOut soundOut waapiOut)
  file(READ "${projectDir}/.eeproj" content)
  string(JSON name GET "${content}" name)
  string(JSON usesSound ERROR_VARIABLE soundError GET "${content}" usesSoundEngine)
  string(JSON usesWaapi ERROR_VARIABLE waapiError GET "${content}" usesWaapi)
  if(soundError)
    set(usesSound FALSE)
  endif()
  if(waapiError)
    set(usesWaapi FALSE)
  endif()
  set(${nameOut} "${name}" PARENT_SCOPE)
  set(${soundOut} "${usesSound}" PARENT_SCOPE)
  set(${waapiOut} "${usesWaapi}" PARENT_SCOPE)
endfunction()

function(ee_add_project target projectDir useSoundEngine useWaapi isTests projectName safeName)
  file(GLOB_RECURSE projectSources CONFIGURE_DEPENDS
    "${projectDir}/Source/*.cpp"
    "${projectDir}/Source/*.h"
    "${projectDir}/Source/*.inl"
    "${projectDir}/Source/*.vert"
    "${projectDir}/Source/*.frag"
  )

  add_executable(${target} ${projectSources})
  ee_common_settings(${target})
  set_target_properties(${target} PROPERTIES
    WIN32_EXECUTABLE "$<$<CONFIG:Dist>:ON>"
    VS_DEBUGGER_WORKING_DIRECTORY "${projectDir}"
    DEBUGGER_WORKING_DIRECTORY "${projectDir}"
  )

  target_compile_definitions(${target} PRIVATE EE_NO_SOUNDENGINE)
  target_link_libraries(${target} PRIVATE ElevateEngine)
  target_include_directories(${target} PRIVATE
    "${CMAKE_SOURCE_DIR}/ElevateEngine/Source"
    "${CMAKE_SOURCE_DIR}/ElevateEngine/Vendor/entt/include"
    "${CMAKE_SOURCE_DIR}/ElevateEngine/Vendor/ImGui"
    "${CMAKE_SOURCE_DIR}/ElevateEngine/Vendor/glm"
    "${CMAKE_SOURCE_DIR}/ElevateEngine/Vendor/spdlog/include"
  )

  if(WIN32)
    target_link_libraries(${target} PRIVATE ws2_32)
  elseif(NOT EMSCRIPTEN)
    target_link_libraries(${target} PRIVATE Xrandr Xinerama Xcursor Xi m)
  endif()

  if(useSoundEngine)
    target_compile_definitions(${target} PRIVATE EE_USES_WWISE)
  endif()

  if(isTests)
    target_include_directories(${target} PRIVATE
      "${CMAKE_SOURCE_DIR}/ElevateTests/Vendor/_config_headers"
      "${CMAKE_SOURCE_DIR}/ElevateTests/Vendor/Catch2/src"
    )
    target_link_libraries(${target} PRIVATE Catch2::Catch2WithMain)
  endif()

  if(EMSCRIPTEN AND NOT isTests)
    include(Web)
    ee_web_link_settings(${target} "${projectDir}" TRUE)
    ee_web_generate_html("${projectName}" "${safeName}")
  endif()
endfunction()
