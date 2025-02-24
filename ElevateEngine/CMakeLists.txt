cmake_minimum_required(VERSION 3.20)
project(ElevateEngine)

#add_library(ElevateEngine STATIC)

#file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
#file(GLOB_RECURSE ENGINE_HEADERS "src/*.h")
#target_sources(ElevateEngine PRIVATE ${ENGINE_SOURCES} ${ENGINE_HEADERS})

set(_src_root_path "${ElevateEngine_SOURCE_DIR}/src")

# Récupérer tous les fichiers source (.cpp, .h) dans le répertoire spécifié
file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    "${_src_root_path}/*.c*"
    "${_src_root_path}/*.h*"
)

# Créer la bibliothèque ElevateEngine avec les fichiers récupérés
add_library(ElevateEngine ${_source_list})

# Ajouter des groupes pour organiser les fichiers dans Visual Studio
foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

# Ajouter les fichiers sources à la cible ElevateEngine
target_sources(ElevateEngine PRIVATE ${_source_list})

# ImGui
set(IMGUI_SRC
    vendor/ImGui/imgui.cpp
    vendor/ImGui/imgui_draw.cpp
    vendor/ImGui/imgui_tables.cpp
    vendor/ImGui/imgui_widgets.cpp
    vendor/ImGui/backends/imgui_impl_opengl3.cpp
    vendor/ImGui/backends/imgui_impl_glfw.cpp

    # ImGuizmo
    vendor/ImGuizmo/ImGuizmo.cpp
)

set(IMGUI_HEADERS
    vendor/ImGui/imgui.h
    vendor/ImGui/imconfig.h
    vendor/ImGui/imgui_internal.h
    vendor/ImGui/imstb_rectpack.h
    vendor/ImGui/imstb_textedit.h
    vendor/ImGui/imstb_truetype.h
    vendor/ImGui/backends/imgui_impl_opengl3.h
    vendor/ImGui/backends/imgui_impl_glfw.h

    # ImGuizmo
    vendor/ImGuizmo/ImGuizmo.h
)

target_sources(ElevateEngine PRIVATE ${IMGUI_SRC} ${IMGUI_HEADERS})

# Glad
add_library(glad STATIC vendor/Glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include)
set_target_properties(glad PROPERTIES FOLDER "Dependencies")

target_include_directories(ElevateEngine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/rapidjson/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tinyfiledialogs
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGuizmo
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/entt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui
)

target_link_libraries(ElevateEngine PUBLIC 
    glfw
    glad
    assimp
    opengl32
)

add_subdirectory(vendor/GLFW)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable Assimp unit tests")
add_subdirectory(vendor/assimp)

#set_target_properties(glad PROPERTIES FOLDER "Dependencies")
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
set_target_properties(assimp PROPERTIES FOLDER "Dependencies")

target_compile_definitions(ElevateEngine PUBLIC GLFW_INCLUDE_NONE)

if(WIN32)
    target_compile_definitions(ElevateEngine PUBLIC EE_PLATFORM_WINDOWS)
endif()