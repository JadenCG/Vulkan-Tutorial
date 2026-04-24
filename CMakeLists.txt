cmake_minimum_required(VERSION 3.31)
project(LearnVulkan)

set(CMAKE_CXX_STANDARD 20)

#glfw library (v3.4 by default!)
#add_subdirectory(glfw)
#include_directories(glfw/include)

#glm library (v1.0.3 by default!)
#add_subdirectory(glm)
#include_directories(glm/glm/gtc)

#find_package(Vulkan REQUIRED)
#find_package(glm REQUIRED)
#find_package(glfw REQUIRED)

#target_link_libraries(${CMAKE_PROJECT_NAME} glm glfw)

#add_executable(LearnVulkan main.cpp)


find_package(Vulkan REQUIRED)
add_subdirectory(glfw)
include_directories(glfw/include)

#TODO: for below: this root/glm
set(glm_DIR ${CMAKE_SOURCE_DIR}/glm)
find_package(glm REQUIRED)

add_executable(foobar)

target_sources(foobar PRIVATE main.cpp)

target_link_libraries(foobar PRIVATE Vulkan::Vulkan)
