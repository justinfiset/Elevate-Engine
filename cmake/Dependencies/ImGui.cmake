file(GLOB imgui_sources
  "${VENDOR_DIR}/ImGui/*.cpp"
  "${VENDOR_DIR}/ImGui/backends/imgui_impl_glfw.cpp"
  "${VENDOR_DIR}/ImGui/backends/imgui_impl_opengl3.cpp"
  "${VENDOR_DIR}/ImGui/misc/cpp/imgui_stdlib.cpp"
)
add_library(ImGui STATIC ${imgui_sources})
target_include_directories(ImGui PUBLIC "${VENDOR_DIR}/ImGui" PRIVATE "${VENDOR_DIR}/Glad/include" "${VENDOR_DIR}/GLFW/include")
target_compile_definitions(ImGui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
if(WIN32)
  target_compile_definitions(ImGui PRIVATE _IMGUI_WIN32 _CRT_SECURE_NO_WARNINGS)
elseif(NOT EMSCRIPTEN)
  target_compile_definitions(ImGui PRIVATE _IMGUI_X11)
  set_target_properties(ImGui PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
if(EMSCRIPTEN)
  ee_web_compile_settings(ImGui)
endif()
ee_setup_outputs(ImGui)
