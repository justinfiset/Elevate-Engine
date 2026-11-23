file(GLOB projectDirs LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/Projects/*")
list(SORT projectDirs)

foreach(projectDir IN LISTS projectDirs)
  if(NOT IS_DIRECTORY "${projectDir}")
    continue()
  endif()
  if(NOT EXISTS "${projectDir}/.eeproj")
    continue()
  endif()

  ee_parse_eeproj("${projectDir}" projectName usesSound usesWaapi)
  ee_safe_project_name("${projectName}" safeName)

  message(STATUS "Loading user project: ${projectDir} (${safeName})")
  ee_add_project("${safeName}" "${projectDir}" "${usesSound}" "${usesWaapi}" FALSE "${projectName}" "${safeName}")

  if(EXISTS "${projectDir}/project.cmake")
    include("${projectDir}/project.cmake")
  endif()
endforeach()
