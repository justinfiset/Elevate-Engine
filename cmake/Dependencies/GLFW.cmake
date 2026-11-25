# Emscripten supplies GLFW through -s USE_GLFW=3.
if(NOT EMSCRIPTEN)
  set(glfw_sources
    "${VENDOR_DIR}/GLFW/src/context.c"
    "${VENDOR_DIR}/GLFW/src/init.c"
    "${VENDOR_DIR}/GLFW/src/input.c"
    "${VENDOR_DIR}/GLFW/src/monitor.c"
    "${VENDOR_DIR}/GLFW/src/vulkan.c"
    "${VENDOR_DIR}/GLFW/src/window.c"
    "${VENDOR_DIR}/GLFW/src/platform.c"
    "${VENDOR_DIR}/GLFW/src/null_init.c"
    "${VENDOR_DIR}/GLFW/src/null_monitor.c"
    "${VENDOR_DIR}/GLFW/src/null_window.c"
    "${VENDOR_DIR}/GLFW/src/null_joystick.c"
  )
  if(WIN32)
    list(APPEND glfw_sources
      "${VENDOR_DIR}/GLFW/src/win32_init.c"
      "${VENDOR_DIR}/GLFW/src/win32_joystick.c"
      "${VENDOR_DIR}/GLFW/src/win32_monitor.c"
      "${VENDOR_DIR}/GLFW/src/win32_time.c"
      "${VENDOR_DIR}/GLFW/src/win32_thread.c"
      "${VENDOR_DIR}/GLFW/src/win32_window.c"
      "${VENDOR_DIR}/GLFW/src/wgl_context.c"
      "${VENDOR_DIR}/GLFW/src/egl_context.c"
      "${VENDOR_DIR}/GLFW/src/osmesa_context.c"
      "${VENDOR_DIR}/GLFW/src/win32_module.c"
    )
  else()
    list(APPEND glfw_sources
      "${VENDOR_DIR}/GLFW/src/x11_init.c"
      "${VENDOR_DIR}/GLFW/src/x11_monitor.c"
      "${VENDOR_DIR}/GLFW/src/x11_window.c"
      "${VENDOR_DIR}/GLFW/src/xkb_unicode.c"
      "${VENDOR_DIR}/GLFW/src/posix_time.c"
      "${VENDOR_DIR}/GLFW/src/posix_thread.c"
      "${VENDOR_DIR}/GLFW/src/posix_module.c"
      "${VENDOR_DIR}/GLFW/src/posix_poll.c"
      "${VENDOR_DIR}/GLFW/src/glx_context.c"
      "${VENDOR_DIR}/GLFW/src/egl_context.c"
      "${VENDOR_DIR}/GLFW/src/osmesa_context.c"
      "${VENDOR_DIR}/GLFW/src/linux_joystick.c"
    )
  endif()

  add_library(GLFW STATIC ${glfw_sources})
  target_include_directories(GLFW PUBLIC "${VENDOR_DIR}/GLFW/include" PRIVATE "${VENDOR_DIR}/GLFW/src")
  if(WIN32)
    target_compile_definitions(GLFW PRIVATE _GLFW_WIN32 _CRT_SECURE_NO_WARNINGS)
  else()
    target_compile_definitions(GLFW PRIVATE _GLFW_X11)
    set_target_properties(GLFW PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif()
  set_target_properties(GLFW PROPERTIES C_STANDARD 11)
  ee_setup_outputs(GLFW)
endif()
