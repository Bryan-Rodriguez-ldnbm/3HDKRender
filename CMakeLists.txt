cmake_minimum_required(VERSION 3.5)

project(3HDK)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-pedantic -Wno-c++11-extensions")

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/src)
set(PROJECT_EXT_DIR ${PROJECT_DIR}/ext)
set(PROJECT_GLAD_DIR ${PROJECT_EXT_DIR}/glad)

file(GLOB_RECURSE PROJECT_SOURCES
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/*.c"
    "${PROJECT_EXT_DIR}/*/*.cpp"
    "${PROJECT_GLAD_DIR}/src/*.c"
)

file(GLOB_RECURSE PROJECT_HEADERS
    "${PROJECT_EXT_DIR}/*/*.h"
    "${PROJECT_SOURCE_DIR}/*.h"
)

include_directories(${PROJECT_EXT_DIR})
include_directories(${PROJECT_GLAD_DIR}/include)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# Find and link GLFW
find_package(glfw3 3.2 REQUIRED)
if (glfw3_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
else()
    message(FATAL_ERROR "GLFW3 not found")
endif()

# Find and link GLM
find_package(glm REQUIRED)
if (glm_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "GLM not found")
endif()

if (APPLE)
    # Add required frameworks for macOS
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
endif()