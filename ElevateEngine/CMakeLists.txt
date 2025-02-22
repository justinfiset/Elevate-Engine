cmake_minimum_required(VERSION 3.20)
project(ElevateEngine)

add_library(ElevateEngine STATIC)

# Ajouter les fichiers sources et headers manuellement ou sans utiliser `**`
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "src/*.h")

# Ajouter les fichiers sources et headers
target_sources(ElevateEngine PRIVATE ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Ajouter les répertoires d'inclusion
target_include_directories(ElevateEngine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/rapidjson/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tinyfiledialogs
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGuizmo
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/entt/include
)

# Lier les bibliothèques
target_link_libraries(ElevateEngine PUBLIC 
    glfw
    glad
    imgui
    assimp
    tinyfiledialogs
    opengl32
)

# Compiler les dépendances (GLFW, Glad, ImGui, etc.)
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/Glad)
add_subdirectory(vendor/ImGui)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/tinyfiledialogs)