cmake_minimum_required(VERSION 3.0)

project(myopenglapp)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/src)

file(GLOB PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/*.cpp" "${PROJECT_SOURCE_DIR}/*.c")

find_package(glfw3 3.2 REQUIRED)
if(NOT glfw3_FOUND)
    message(FATAL_ERROR "GLFW3 not found")
endif()

find_package(glm REQUIRED)
if (NOT glm_FOUND)
    message(FATAL_ERROR "GLM not found")
endif()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} glfw)
