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

if(EE_CI_SCCACHE)
  # Only vendored C/C++ code is wrapped. Module interface units are excluded
  # because their BMIs are not safe to reuse through a compiler launcher.
  foreach(target IN ITEMS GLFW ImGui Jolt assimp)
    if(TARGET ${target})
      set_target_properties(${target} PROPERTIES
        C_COMPILER_LAUNCHER sccache
        CXX_COMPILER_LAUNCHER sccache
      )
    endif()
  endforeach()
endif()
