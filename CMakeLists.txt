cmake_minimum_required(VERSION 3.7.2)

if(NOT DEFINED PROJECT_VERSION_MAJOR)
  set(PROJECT_VERSION_MAJOR 0)
endif()

if(NOT DEFINED PROJECT_VERSION_MINOR)
  set(PROJECT_VERSION_MINOR 0)
endif()

if(NOT DEFINED PATCH_VERSION)
  set(PATCH_VERSION 0)
endif()

set(PROJECT_VERSION_PATCH ${PATCH_VERSION})

project(physics VERSION
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

configure_file(version.h.in version.h @ONLY) 

set(CMAKE_CXX_STANDARD 17)

# Пытаемся найти предустановленный SFML
find_package(SFML COMPONENTS graphics)

if (SFML_FOUND)
    message(STATUS "The SFML installed in the system will be used")
else ()
    # Если SFML не найден - выкачиваем и собираем из исходников
    message(STATUS "Get SFML from the source code")
    include(FetchContent)
    FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.x)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    # Обход потенциальных проблем при сборке sfml-graphics
    # (stb_image.h: 'argument': conversion from 'int' to 'short')
    set(WARNINGS_AS_ERRORS FALSE CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SFML)
endif ()

# К сожалению, вплоть до C++20 https://en.cppreference.com/w/cpp/numeric/constants
# нет удобного кроссплатформенного способа получить число pi.
# define ниже нужен для включения M_PI
if (MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
endif ()

add_executable(physics
        main.cpp World.cpp Ball.cpp Physics.cpp Color.cpp Painter.cpp
        black_box/Application.cpp black_box/View.cpp black_box/PainterImpl.cpp Dust.cpp)
target_link_libraries(physics PRIVATE sfml-graphics)

target_compile_options(physics PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
        )

set(CPACK_PACKAGE_NAME "physics")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

if(WIN32)
  set(CPACK_GENERATOR "ZIP")
elseif(UNIX AND NOT APPLE)
  set(CPACK_GENERATOR "TGZ")
endif()

install(TARGETS physics DESTINATION bin)

set(CPACK_PACKAGE_NAME "physics")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/package")

include(CPack)
