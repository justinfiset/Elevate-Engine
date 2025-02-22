cmake_minimum_required(VERSION 3.20)
project(Sandbox)

add_executable(Sandbox)

# Ajouter les fichiers sources et headers
file(GLOB_RECURSE SANDBOX_SOURCES "src/*.cpp")
file(GLOB_RECURSE SANDBOX_HEADERS "src/*.h")

target_sources(Sandbox PRIVATE ${SANDBOX_SOURCES} ${SANDBOX_HEADERS})

# Ajouter les répertoires d'inclusion
target_include_directories(Sandbox PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../ElevateEngine/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../ElevateEngine/vendor
)

# Lier avec le moteur
target_link_libraries(Sandbox PRIVATE ElevateEngine)
