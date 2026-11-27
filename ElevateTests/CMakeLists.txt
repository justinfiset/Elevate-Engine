set(CATCH_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_HELPERS OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vendor/Catch2" "${CMAKE_BINARY_DIR}/catch2" EXCLUDE_FROM_ALL)
set_target_properties(Catch2 Catch2WithMain PROPERTIES FOLDER "Tests/Catch2")

ee_parse_eeproj("${CMAKE_CURRENT_SOURCE_DIR}" projectName usesSound usesWaapi)
ee_safe_project_name("${projectName}" safeName)

ee_add_project("${safeName}" "${CMAKE_CURRENT_SOURCE_DIR}" "${usesSound}" "${usesWaapi}" TRUE "${projectName}" "${safeName}")

add_test(NAME ElevateEngine-Tests COMMAND "${safeName}")
