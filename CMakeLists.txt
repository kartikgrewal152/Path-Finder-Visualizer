cmake_minimum_required(VERSION 3.15)
project(PathFinderVisualizer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------------
# SFML 2.6.x  (window, graphics, system)
# --------------------------------------------------------------------------
set(SFML_BUILD_AUDIO   OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS  ON  CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.1
    GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(SFML)

# --------------------------------------------------------------------------
# Dear ImGui (source-only, no CMake support upstream -> build manually)
# --------------------------------------------------------------------------
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.90.4
    GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

# --------------------------------------------------------------------------
# ImGui-SFML (bridges Dear ImGui <-> SFML rendering / input)
# --------------------------------------------------------------------------
set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "" FORCE)
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG        v2.6
    GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(imgui-sfml)

# --------------------------------------------------------------------------
# Application target
# --------------------------------------------------------------------------
add_executable(PathFinderVisualizer
    src/main.cpp
    src/Grid.cpp
    src/Algorithms.cpp
    src/UIManager.cpp
)

target_include_directories(PathFinderVisualizer PRIVATE include)

target_link_libraries(PathFinderVisualizer PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    ImGui-SFML::ImGui-SFML
)

# Copy SFML shared libraries next to the executable on Windows
if(WIN32)
    add_custom_command(TARGET PathFinderVisualizer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE_DIR:PathFinderVisualizer>
    )
endif()
