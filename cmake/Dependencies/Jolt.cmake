file(GLOB_RECURSE jolt_sources CONFIGURE_DEPENDS "${VENDOR_DIR}/JoltPhysics/Jolt/*.cpp")
list(FILTER jolt_sources EXCLUDE REGEX "/(Compute/(CPU|DX12|VK|MTL)|ObjectStream)/")
list(APPEND jolt_sources "${VENDOR_DIR}/JoltPhysics/Jolt/ObjectStream/SerializableObject.cpp")
add_library(Jolt STATIC ${jolt_sources})
ee_source_group_tree("${VENDOR_DIR}/JoltPhysics" "Vendor/JoltPhysics" ${jolt_sources})
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
