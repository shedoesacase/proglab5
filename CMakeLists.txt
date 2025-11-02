cmake_minimum_required(VERSION 3.20)

project(physics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
endif ()

find_package(SFML COMPONENTS graphics window system QUIET)

if (SFML_FOUND)
    message(STATUS "Using system SFML")
else ()
    message(STATUS "Fetching SFML source via FetchContent")

    include(FetchContent)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
    )

    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
    set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

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

target_link_libraries(physics PRIVATE sfml-graphics sfml-window sfml-system)

target_compile_options(physics PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

install(TARGETS physics DESTINATION bin)

include(CPack)
set(CPACK_PACKAGE_NAME "physics")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "ZIP")
