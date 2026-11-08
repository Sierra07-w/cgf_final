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

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    URL https://codeload.github.com/glfw/glfw/zip/refs/tags/3.4
    URL_HASH SHA256=A133DDC3D3C66143EBA9035621DB8E0BCF34DBA1EE9514A9E23E96AFD39FD57A
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
    glm
    URL https://codeload.github.com/g-truc/glm/zip/refs/tags/1.0.1
    URL_HASH SHA256=09C5716296787E1F7FCB87B1CBDBF26814EC1288ED6259CCD30D5D9795809FA5
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
    imgui
    URL https://codeload.github.com/ocornut/imgui/zip/refs/tags/v1.93.0
    URL_HASH SHA256=E21907FD8DD5EA59AA00538632A02AD0A3EDEDB4DDC533B94702331FBCD6C470
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(glfw glm imgui)

add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include")

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw)

add_executable(cozy_room
    src/app/main.cpp
    src/gfx/Shader.cpp
    src/scene/Camera.cpp
    src/scene/Lighting.cpp
    src/scene/Room.cpp
)

target_include_directories(cozy_room PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(cozy_room PRIVATE
    glad
    glfw
    glm::glm
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
