function(ee_web_link_settings target projectDir isEditorCapable)
  target_link_options(${target} PRIVATE
    "SHELL:-pthread -matomics -mbulk-memory"
    "SHELL:-s STACK_SIZE=5MB"
    "SHELL:-s PTHREAD_POOL_SIZE=8"
    "SHELL:-s USE_GLFW=3"
    "SHELL:-s FULL_ES3=1"
    "SHELL:-s ASSERTIONS=0"
    "SHELL:-s EMULATE_FUNCTION_POINTER_CASTS=1"
    "SHELL:-s NO_EXIT_RUNTIME=1"
    "SHELL:-s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=['FS','wasmTable','ccall','cwrap']"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    "SHELL:-s GROWABLE_ARRAYBUFFERS=0"
    "SHELL:-s INITIAL_MEMORY=512MB"
    "SHELL:-s FORCE_FILESYSTEM=1"
    "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"
    "SHELL:-qext"
  )

  set(engineResources "${CMAKE_SOURCE_DIR}/ElevateEngine/Resources/Engine")
  set(editorResources "${CMAKE_SOURCE_DIR}/ElevateEngine/Resources/Editor")
  set(projectContent "${projectDir}/Content")
  set(projectConfig "${projectDir}/app.config")

  # Each preload must stay in a single SHELL: group. CMake deduplicates
  # repeated bare "--preload-file" entries, which turns later paths into
  # linker inputs instead of Emscripten preload arguments.
  foreach(pair IN ITEMS
      "${engineResources}|/Engine"
      "${projectContent}|/Content"
      "${projectConfig}|/app.config"
    )
    string(REPLACE "|" ";" pair "${pair}")
    list(GET pair 0 sourcePath)
    list(GET pair 1 mountPath)
    if(EXISTS "${sourcePath}")
      target_link_options(${target} PRIVATE "SHELL:--preload-file ${sourcePath}@${mountPath}")
    endif()
  endforeach()

  if(EXISTS "${projectDir}/imgui.ini")
    target_link_options(${target} PRIVATE
      "SHELL:--preload-file ${projectDir}/imgui.ini@/imgui.ini"
    )
  endif()

  if(isEditorCapable AND EXISTS "${editorResources}")
    foreach(cfg Editor_Debug Editor_Release)
      target_link_options(${target} PRIVATE
        "SHELL:$<$<CONFIG:${cfg}>:--preload-file ${editorResources}@/Editor>"
      )
    endforeach()
  endif()
endfunction()

function(ee_web_generate_html projectName safeName)
  set(template "${CMAKE_SOURCE_DIR}/ElevateEngine/Web/Templates/index.template.html")
  set(assets "${CMAKE_SOURCE_DIR}/ElevateEngine/Web/Assets")
  file(READ "${template}" content)
  string(REPLACE "\${PROJECT_NAME}" "${projectName}" content "${content}")
  string(REPLACE "\${PROJECT_SAFE_NAME}" "${safeName}" content "${content}")

  foreach(cfg IN LISTS ELEVATE_CONFIGS)
    set(outputDir "${CMAKE_SOURCE_DIR}/Build/bin/${cfg}-${EE_PLATFORM_TAG}-${EE_ARCH_TAG}/${safeName}")
    file(MAKE_DIRECTORY "${outputDir}")
    file(WRITE "${outputDir}/index.html" "${content}")
    if(IS_DIRECTORY "${assets}")
      file(COPY "${assets}/" DESTINATION "${outputDir}")
    endif()
  endforeach()
endfunction()
