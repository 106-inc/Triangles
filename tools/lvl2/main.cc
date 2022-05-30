#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <cstdlib>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

const std::vector<const char *> VALIDATION_LAYERS{"VK_LAYER_KHRONOS_validation"};
const std::vector<const char *> DEVICE_EXTENSIONS{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator,
                                      VkDebugUtilsMessengerEXT *pCallback)
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
    instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr)
    return func(instance, pCreateInfo, pAllocator, pCallback);
  else
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator)
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
    instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr)
    func(instance, debugMessenger, pAllocator);
}

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete()
  {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

class HelloTriangleApplication
{
public:
  void run()
  {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window_;

  vk::UniqueInstance instance_;
  vk::DebugUtilsMessengerEXT debugMessenger_;
  vk::SurfaceKHR surface_;

  vk::PhysicalDevice physicalDevice_;
  vk::UniqueDevice device_;

  vk::Queue graphicsQueue_;
  vk::Queue presentQueue_;

  void initWindow()
  {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  }

  void initVulkan()
  {
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
  }

  void mainLoop()
  {
    while (!glfwWindowShouldClose(window_))
    {
      glfwPollEvents();
    }
  }

  void cleanup()
  {
    if constexpr (enableValidationLayers)
      DestroyDebugUtilsMessengerEXT(*instance_, debugMessenger_, nullptr);

    instance_->destroySurfaceKHR(surface_);

    glfwDestroyWindow(window_);
    glfwTerminate();
  }

  void createInstance()
  {
    if (enableValidationLayers && !checkValidationLayerSupport())
      throw std::runtime_error("validation layers requested, but not available!");

    vk::ApplicationInfo appInfo{.pApplicationName = "Hello triangle",
                                .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                                .pEngineName = "No engine",
                                .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                                .apiVersion = VK_API_VERSION_1_0};

    auto extensions = getRequiredExtensions();
    vk::InstanceCreateInfo createInfo{.pApplicationInfo = &appInfo,
                                      .enabledLayerCount = 0,
                                      .enabledExtensionCount =
                                        static_cast<uint32_t>(extensions.size()),
                                      .ppEnabledExtensionNames = extensions.data()};

    vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if constexpr (enableValidationLayers)
    {
      createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
      createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();

      populateDebugMessengerCreateInfo(debugCreateInfo);
      vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT>
        structureChain = {createInfo, debugCreateInfo};
    }

    instance_ = vk::createInstanceUnique(createInfo);

    std::cout << "available extensions:\n";
    for (const auto &extension : vk::enumerateInstanceExtensionProperties())
      std::cout << '\t' << extension.extensionName << std::endl;
  }

  void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo)
  {
    createInfo = {.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                                     vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                     vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                  .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                 vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                                 vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
                  .pfnUserCallback = &debugCallback};
  }

  void setupDebugMessenger()
  {
    if constexpr (!enableValidationLayers)
      return;

    vk::DispatchLoaderDynamic dld{*instance_, vkGetInstanceProcAddr};
    vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    debugMessenger_ = instance_->createDebugUtilsMessengerEXT(createInfo, nullptr, dld);
  }

  void createSurface()
  {
    VkSurfaceKHR rawSurface;
    if (glfwCreateWindowSurface(*instance_, window_, nullptr, &rawSurface) != VK_SUCCESS)
      throw std::runtime_error("failed to create window surface!");
    surface_ = rawSurface;
  }

  void pickPhysicalDevice()
  {
    auto devices = instance_->enumeratePhysicalDevices();
    if (0 == devices.size())
      throw std::runtime_error("failed to find GPUs with Vulkan support!");

    for (const auto &device : devices)
      if (isDeviceSuitable(device))
      {
        physicalDevice_ = device;
        break;
      }

    if (!physicalDevice_)
      throw std::runtime_error("failed to find a suitable GPU!");
  }

  void createLogicalDevice()
  {
    auto indices = findQueueFamilies(physicalDevice_);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{};
    std::set<uint32_t> uniqueQueueFamilies{indices.graphicsFamily.value(),
                                           indices.presentFamily.value()};

    float queuePriority = 1.0;
    for (auto queueFamily : uniqueQueueFamilies)
    {
      vk::DeviceQueueCreateInfo queueCreateInfo{
        .queueFamilyIndex = queueFamily, .queueCount = 1, .pQueuePriorities = &queuePriority};
      queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures{};
    vk::DeviceCreateInfo createInfo{.queueCreateInfoCount = queueCreateInfos.size(),
                                    .pQueueCreateInfos = queueCreateInfos.data(),
                                    .enabledExtensionCount = 0,
                                    .pEnabledFeatures = &deviceFeatures};

    if constexpr (enableValidationLayers)
    {
      createInfo.setEnabledLayerCount(VALIDATION_LAYERS.size());
      createInfo.setPpEnabledLayerNames(VALIDATION_LAYERS.data());
    }

    device_ = physicalDevice_.createDeviceUnique(createInfo);
    graphicsQueue_ = device_->getQueue(indices.graphicsFamily.value(), 0);
    presentQueue_ = device_->getQueue(indices.presentFamily.value(), 0);
  }

  QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device)
  {
    int i = 0;
    QueueFamilyIndices indices{};
    auto queueFamilies = device.getQueueFamilyProperties();
    for (const auto &queueFamily : queueFamilies)
    {
      if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        indices.graphicsFamily = i;

      if (device.getSurfaceSupportKHR(i, surface_))
        indices.graphicsFamily = i;

      if (indices.isComplete())
        break;

      ++i;
    }

    return indices;
  }

  bool isDeviceSuitable(vk::PhysicalDevice device)
  {
    auto indices = findQueueFamilies(device);
    return indices.isComplete() && checkDeviceExtensionSupport(device);
  }

  bool checkDeviceExtensionSupport(vk::PhysicalDevice device)
  {
    // device.enumerateDeviceExtensionProperties()
    return true;
  }

  std::vector<const char *> getRequiredExtensions()
  {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if constexpr (enableValidationLayers)
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
  }

  bool checkValidationLayerSupport()
  {
    auto availableLayers = vk::enumerateInstanceLayerProperties();

    for (std::string_view layerName : VALIDATION_LAYERS)
    {
      bool layerFound = false;

      for (const auto &layerProperties : availableLayers)
        if (layerProperties.layerName == layerName)
        {
          layerFound = true;
          break;
        }

      if (!layerFound)
        return false;
    }

    return true;
  }
};

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

int main()
try
{
  HelloTriangleApplication app;
  app.run();
  return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
}
catch (...)
{
  std::cerr << "Unkwnown error" << std::endl;
  return EXIT_FAILURE;
}
