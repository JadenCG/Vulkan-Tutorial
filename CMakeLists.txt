cmake_minimum_required(VERSION 3.31)
project(LearnVulkan)

set(CMAKE_CXX_STANDARD 20)

#Vulkan
find_package(Vulkan REQUIRED)

#GLFW (version 3.4 by default)
add_subdirectory(glfw)

#GLM (version 1.0.3 by default)
add_subdirectory(glm)

add_executable(${PROJECT_NAME} src/triangle/main.cpp)

target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan glm::glm)