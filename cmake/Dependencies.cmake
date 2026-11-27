set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/ElevateEngine/Vendor")

include("${CMAKE_CURRENT_LIST_DIR}/Dependencies/VendorHeaders.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Dependencies/GLFW.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Dependencies/ImGui.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Dependencies/Jolt.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Dependencies/Assimp.cmake")

foreach(target IN ITEMS ElevateVendorHeaders GLFW ImGui Jolt assimp)
  if(TARGET ${target})
    ee_target_folder(${target} "Dependencies")
  endif()
endforeach()
