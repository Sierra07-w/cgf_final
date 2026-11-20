cmake_minimum_required(VERSION 3.20)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(CozyRoomDiorama LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/external/glm")

add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include")

add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends"
)

target_link_libraries(imgui PUBLIC glfw)

add_executable(cozy_room
    src/app/main.cpp
    src/gfx/Mesh.cpp
    src/gfx/Shader.cpp
    src/gfx/Texture.cpp
    src/scene/Atmosphere.cpp
    src/scene/Camera.cpp
    src/scene/DayNight.cpp
    src/scene/Lighting.cpp
    src/scene/Primitives.cpp
    src/scene/Room.cpp
    src/scene/Scene.cpp
    src/scene/SceneNode.cpp
)

target_include_directories(cozy_room PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(cozy_room PRIVATE
    glad
    glfw
    glm
    imgui
)

if(WIN32)
    target_link_libraries(cozy_room PRIVATE opengl32)
endif()

if(MINGW)
    target_link_options(cozy_room PRIVATE -static)
endif()

if(MSVC)
    target_compile_options(cozy_room PRIVATE /W4)
else()
    target_compile_options(cozy_room PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_definitions(cozy_room PRIVATE
    SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders"
)
