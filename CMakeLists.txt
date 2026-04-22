cmake_minimum_required(VERSION 3.31)
project(LearnVulkan)

set(CMAKE_CXX_STANDARD 20)

#glfw library (v3.4 by default!)
add_subdirectory(glfw)
include_directories(glfw/include)

#glm library (v1.0.3 by default!)
add_subdirectory(glm/glm)
include_directories(glm/glm)

add_executable(LearnVulkan main.cpp)
