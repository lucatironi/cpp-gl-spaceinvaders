cmake_minimum_required(VERSION 3.0)
project(spaceinvaders)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(vendor/glfw)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(src/
                    vendor/freetype/include/
                    vendor/glad/include/
                    vendor/glfw/include/
                    vendor/glm/
                    vendor/irrKlang/include/
                    vendor/stb/)

if(APPLE)
    find_library(irrKlang_location
                NAMES libirrklang.dylib
                HINTS "vendor/irrKlang/bin/macosx-gcc/")
elseif(WIN32)
    find_library(irrKlang_location
                NAMES irrKlang.dll
                HINTS "vendor/irrKlang/bin/winx64-visualStudio/")
endif()

add_library(irrKlang STATIC IMPORTED)
set_target_properties(irrKlang PROPERTIES IMPORTED_LOCATION ${irrKlang_location})

file(GLOB VENDORS_SOURCES vendor/glad/src/glad.c)
file(GLOB PROJECT_HEADERS src/*.hpp)
file(GLOB PROJECT_SOURCES src/*.cpp)
file(GLOB PROJECT_SHADERS src/shaders/*.vs
                          src/shaders/*.fs)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} glfw irrKlang freetype
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})