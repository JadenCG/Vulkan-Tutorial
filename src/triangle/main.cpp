#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//Stealing stuff from https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Base_code for now
class TriangleClass {
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }
private:
    GLFWwindow* window = nullptr;

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //Resizing disabled for this chapter
        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);

    }
    void initVulkan() {

    }
    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }
    void cleanup() {
        //Technically not necessary since memory will be cleaned on close
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    TriangleClass mainApp;
    mainApp.run();
    return 0;
}