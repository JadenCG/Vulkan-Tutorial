#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <string.h>
#include <optional> //Used for findQueueFamilies()

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
    VkInstance instance;
    VkPhysicalDevice graphicsDevice = VK_NULL_HANDLE; //graphics card we'll be using
    VkDevice device;
    VkQueue graphicsQueue;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete() {
            return graphicsFamily.has_value();
        }
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //Resizing disabled for this chapter
        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan() {
        createVulkanInstance();
        //setupDebugMessenger(); //Skipping for now
        pickPhysicalDevice();
        createLogicalDevice();
    }

    //Picks the graphics card to use
    void pickPhysicalDevice() {
        //Initial check: if there are no graphics cards we can use, throw an exception
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if(deviceCount == 0)
            throw std::runtime_error("failed to find GPUs with Vulkan support!");

        //Check to see which devices support Vulkan
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                graphicsDevice = device;
                break;
            }
        }
        //Throw an exception if graphicsDevice never updated
        if (graphicsDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    //Determines what features the graphics card should use
    void createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamilies(graphicsDevice);

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = 0;

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(graphicsDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    }

    //Helper for pickPhysicalDevice()
    bool isDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indices = findQueueFamilies(device);
        return indices.isComplete();
    }

    //Helper for pickPhysicalDevice()(?)
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        // Logic to find graphics queue family
        QueueFamilyIndices indices;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        //Loop through existing queue families to find relevant features
        //(Currently only looking for the graphics bit queue)
        int currentFamily = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = currentFamily;
            }
            currentFamily++;

            //if (indices.isComplete())
            //    break;
        }

        return indices;
    }

    void createVulkanInstance() {
        //Validation layer checks
        if(enableValidationLayers && !checkValidationLayerSupport())
            throw std::runtime_error("validation layers requested, but not available!");

        //App / Window struct info
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        //Vulkan driver settings struct (Global)
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
            createInfo.enabledLayerCount = 0;

        //Creating the Vulkan instance (no custom allocator)
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr); //No allocator

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