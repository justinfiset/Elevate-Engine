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
  "${VENDOR_DIR}/entt/src"
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
