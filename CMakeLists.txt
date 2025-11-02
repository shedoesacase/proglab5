cmake_minimum_required(VERSION 3.16)

project(physics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/_install" CACHE PATH "Install path" FORCE)

find_package(SFML 2.6 COMPONENTS graphics CONFIG QUIET)

if (SFML_FOUND)
    message(STATUS "Using system SFML")
else ()
    message(STATUS "Fetching SFML from GitHub...")
    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
    )
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(WARNINGS_AS_ERRORS FALSE CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SFML)
endif ()

add_executable(physics
    main.cpp
    World.cpp
    Ball.cpp
    Physics.cpp
    Color.cpp
    Painter.cpp
    Dust.cpp
    black_box/Application.cpp
    black_box/View.cpp
    black_box/PainterImpl.cpp
)

target_link_libraries(physics PRIVATE sfml-graphics)

target_compile_options(physics PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

install(TARGETS physics DESTINATION bin)
