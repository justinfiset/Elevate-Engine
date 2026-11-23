set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/ElevateEngine/Vendor")

add_library(ElevateVendorHeaders INTERFACE)
set(ee_vendor_include_dirs
  "${VENDOR_DIR}"
  "${CMAKE_SOURCE_DIR}/ElevateEngine/Source"
  "${VENDOR_DIR}/GLFW/include"
  "${VENDOR_DIR}/Glad/include"
  "${VENDOR_DIR}/ImGui"
  "${VENDOR_DIR}/glm"
  "${VENDOR_DIR}/rapidjson/include"
  "${VENDOR_DIR}/rapidxml/include"
  "${VENDOR_DIR}/stb"
  "${VENDOR_DIR}/spdlog/include"
  "${VENDOR_DIR}/assimp/include"
  "${VENDOR_DIR}/tinyfiledialogs"
  "${VENDOR_DIR}/ImGuizmo/src"
  "${VENDOR_DIR}/entt/include"
  "${VENDOR_DIR}/stduuid/include"
  "${VENDOR_DIR}/JoltPhysics"
  "${VENDOR_DIR}/_config_headers"
  "${VENDOR_DIR}/_config_headers/assimp"
)
foreach(dir IN LISTS ee_vendor_include_dirs)
  if(IS_DIRECTORY "${dir}")
    target_include_directories(ElevateVendorHeaders INTERFACE "${dir}")
  endif()
endforeach()

# ---------------------------------------------------------------------------
# GLFW (desktop only; Emscripten supplies GLFW through -s USE_GLFW=3)
# ---------------------------------------------------------------------------
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

# ---------------------------------------------------------------------------
# Dear ImGui
# ---------------------------------------------------------------------------
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

# ---------------------------------------------------------------------------
# Jolt Physics
# ---------------------------------------------------------------------------
file(GLOB_RECURSE jolt_sources CONFIGURE_DEPENDS "${VENDOR_DIR}/JoltPhysics/Jolt/*.cpp")
list(FILTER jolt_sources EXCLUDE REGEX "/(Compute/(CPU|DX12|VK|MTL)|ObjectStream)/")
list(APPEND jolt_sources "${VENDOR_DIR}/JoltPhysics/Jolt/ObjectStream/SerializableObject.cpp")
add_library(Jolt STATIC ${jolt_sources})
target_include_directories(Jolt PUBLIC "${VENDOR_DIR}/JoltPhysics")
target_compile_definitions(Jolt PUBLIC
  $<$<CONFIG:Debug>:JPH_ENABLE_ASSERTS>
  $<$<CONFIG:Editor_Debug>:JPH_ENABLE_ASSERTS>
  $<$<CONFIG:Release>:NDEBUG>
  $<$<CONFIG:Editor_Release>:NDEBUG>
  $<$<CONFIG:Dist>:NDEBUG>
)
set_target_properties(Jolt PROPERTIES CXX_STANDARD 17 POSITION_INDEPENDENT_CODE ON)
if(EMSCRIPTEN)
  ee_web_compile_settings(Jolt)
endif()
ee_setup_outputs(Jolt)

# ---------------------------------------------------------------------------
# Assimp (trimmed to the importers used by the engine)
# ---------------------------------------------------------------------------
file(GLOB_RECURSE assimp_sources LIST_DIRECTORIES false
  "${VENDOR_DIR}/assimp/contrib/unzip/*"
  "${VENDOR_DIR}/assimp/contrib/irrXML/*"
  "${VENDOR_DIR}/assimp/code/Common/*"
  "${VENDOR_DIR}/assimp/code/PostProcessing/*"
  "${VENDOR_DIR}/assimp/code/Material/*"
  "${VENDOR_DIR}/assimp/code/CApi/*"
  "${VENDOR_DIR}/assimp/code/Geometry/*"
  "${VENDOR_DIR}/assimp/code/AssetLib/IQM/*"
  "${VENDOR_DIR}/assimp/code/AssetLib/Collada/*"
  "${VENDOR_DIR}/assimp/code/AssetLib/Obj/*"
  "${VENDOR_DIR}/assimp/code/AssetLib/Blender/*"
  "${VENDOR_DIR}/assimp/contrib/poly2tri/poly2tri/*"
  "${VENDOR_DIR}/assimp/code/AssetLib/FBX/*"
  "${VENDOR_DIR}/assimp/code/glTF2/*"
  "${VENDOR_DIR}/assimp/code/glTF/*"
)
file(GLOB assimp_zlib_sources LIST_DIRECTORIES false "${VENDOR_DIR}/assimp/contrib/zlib/*")
list(APPEND assimp_sources ${assimp_zlib_sources})
list(FILTER assimp_sources INCLUDE REGEX "\\.(c|cc|cpp|h|hpp|inl)$")
add_library(assimp STATIC ${assimp_sources})
target_include_directories(assimp PUBLIC
  "${VENDOR_DIR}/_config_headers"
  "${VENDOR_DIR}/_config_headers/assimp"
  "${VENDOR_DIR}/assimp/include"
  "${VENDOR_DIR}/assimp"
  "${VENDOR_DIR}/assimp/contrib"
  "${VENDOR_DIR}/assimp/contrib/irrXML"
  "${VENDOR_DIR}/assimp/contrib/unzip"
  "${VENDOR_DIR}/assimp/contrib/rapidjson/include"
  "${VENDOR_DIR}/assimp/contrib/pugixml/src"
  "${VENDOR_DIR}/assimp/contrib/zlib"
  "${VENDOR_DIR}/assimp/contrib/utf8cpp/source"
  "${VENDOR_DIR}/assimp/code"
)
target_compile_definitions(assimp PUBLIC ASSIMP_BUILD_STATIC RAPIDJSON_HAS_STDSTRING=1)
target_compile_definitions(assimp PRIVATE
  ASSIMP_BUILD_NO_3D_IMPORTER ASSIMP_BUILD_NO_3DS_IMPORTER ASSIMP_BUILD_NO_3MF_IMPORTER
  ASSIMP_BUILD_NO_AC_IMPORTER ASSIMP_BUILD_NO_AMF_IMPORTER ASSIMP_BUILD_NO_ASE_IMPORTER
  ASSIMP_BUILD_NO_USD_IMPORTER ASSIMP_BUILD_NO_PBRT_IMPORTER ASSIMP_BUILD_NO_ASSBIN_IMPORTER
  ASSIMP_BUILD_NO_B3D_IMPORTER ASSIMP_BUILD_NO_BLEND_IMPORTER ASSIMP_BUILD_NO_BVH_IMPORTER
  ASSIMP_BUILD_NO_C4D_IMPORTER ASSIMP_BUILD_NO_COB_IMPORTER ASSIMP_BUILD_NO_COLLADA_IMPORTER
  ASSIMP_BUILD_NO_CSM_IMPORTER ASSIMP_BUILD_NO_DXF_IMPORTER ASSIMP_BUILD_NO_GLTF_IMPORTER
  ASSIMP_BUILD_NO_HMP_IMPORTER ASSIMP_BUILD_NO_IFC_IMPORTER ASSIMP_BUILD_NO_IRR_IMPORTER
  ASSIMP_BUILD_NO_IRRMESH_IMPORTER ASSIMP_BUILD_NO_LWO_IMPORTER ASSIMP_BUILD_NO_LWS_IMPORTER
  ASSIMP_BUILD_NO_M3D_IMPORTER ASSIMP_BUILD_NO_MD2_IMPORTER ASSIMP_BUILD_NO_MD3_IMPORTER
  ASSIMP_BUILD_NO_MD5_IMPORTER ASSIMP_BUILD_NO_MDC_IMPORTER ASSIMP_BUILD_NO_MDL_IMPORTER
  ASSIMP_BUILD_NO_MMD_IMPORTER ASSIMP_BUILD_NO_MS3D_IMPORTER ASSIMP_BUILD_NO_NDO_IMPORTER
  ASSIMP_BUILD_NO_NFF_IMPORTER ASSIMP_BUILD_NO_OFF_IMPORTER ASSIMP_BUILD_NO_OGRE_IMPORTER
  ASSIMP_BUILD_NO_OPENGEX_IMPORTER ASSIMP_BUILD_NO_PLY_IMPORTER ASSIMP_BUILD_NO_Q3BSP_IMPORTER
  ASSIMP_BUILD_NO_Q3D_IMPORTER ASSIMP_BUILD_NO_RAW_IMPORTER ASSIMP_BUILD_NO_SIB_IMPORTER ASSIMP_BUILD_NO_SMD_IMPORTER
  ASSIMP_BUILD_NO_STEP_IMPORTER ASSIMP_BUILD_NO_STL_IMPORTER ASSIMP_BUILD_NO_TERRAGEN_IMPORTER
  ASSIMP_BUILD_NO_X_IMPORTER ASSIMP_BUILD_NO_X3D_IMPORTER ASSIMP_BUILD_NO_XGL_IMPORTER
  ASSIMP_BUILD_NO_COLLADA_EXPORTER ASSIMP_BUILD_NO_X_EXPORTER ASSIMP_BUILD_NO_STEP_EXPORTER
  ASSIMP_BUILD_NO_OBJ_EXPORTER ASSIMP_BUILD_NO_STL_EXPORTER ASSIMP_BUILD_NO_PLY_EXPORTER
  ASSIMP_BUILD_NO_3DS_EXPORTER ASSIMP_BUILD_NO_GLTF_EXPORTER ASSIMP_BUILD_NO_ASSBIN_EXPORTER
  ASSIMP_BUILD_NO_ASSXML_EXPORTER ASSIMP_BUILD_NO_X3D_EXPORTER ASSIMP_BUILD_NO_FBX_EXPORTER
  ASSIMP_BUILD_NO_M3D_EXPORTER ASSIMP_BUILD_NO_3MF_EXPORTER ASSIMP_BUILD_NO_ASSJSON_EXPORTER
  ASSIMP_BUILD_NO_USD_EXPORTER ASSIMP_BUILD_NO_PBRT_EXPORTER
)
if(EMSCRIPTEN)
  target_compile_definitions(assimp PRIVATE HAVE_UNISTD_H)
  ee_web_compile_settings(assimp)
  foreach(source IN LISTS assimp_sources)
    if(source MATCHES "/poly2tri/.*\\.cc$")
      set_property(SOURCE "${source}" APPEND PROPERTY COMPILE_OPTIONS "-include" "ostream")
    endif()
  endforeach()
elseif(UNIX)
  foreach(source IN LISTS assimp_sources)
    if(source MATCHES "/contrib/zlib/")
      set_property(SOURCE "${source}" APPEND PROPERTY COMPILE_OPTIONS "-include" "unistd.h")
    endif()
  endforeach()
endif()
set_target_properties(assimp PROPERTIES CXX_STANDARD 17 POSITION_INDEPENDENT_CODE ON)
if(MSVC)
  target_compile_options(assimp PRIVATE /W0)
else()
  target_compile_options(assimp PRIVATE -w)
endif()
ee_setup_outputs(assimp)
