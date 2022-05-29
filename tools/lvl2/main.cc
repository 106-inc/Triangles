#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <cstdlib>
#include <iostream>
#include <optional>
#include <stdexcept>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

const std::vector<const char *> validationLayers{"VK_LAYER_KHRONOS_validation"};

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

  bool isComplete()
  {
    return graphicsFamily.has_value();
  }

  uint32_t value()
  {
    return graphicsFamily.value();
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
  VkDebugUtilsMessengerEXT debugMessenger_; // move to vulkan.hpp somehow...

  vk::PhysicalDevice physicalDevice_;
  vk::UniqueDevice device_;

  vk::Queue graphicsQueue_;

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
      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();

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
                  .pfnUserCallback = debugCallback};
  }

  void setupDebugMessenger()
  {
    if constexpr (!enableValidationLayers)
      return;

    vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    auto createDebugRes = CreateDebugUtilsMessengerEXT(
      *instance_, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT *>(&createInfo),
      nullptr, &debugMessenger_);
    if (createDebugRes != VK_SUCCESS)
      throw std::runtime_error("failed to set up debug callback!");
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

    float queuePriority = 1.0;
    vk::DeviceQueueCreateInfo queueCreateInfo{
      .queueFamilyIndex = indices.value(), .queueCount = 1, .pQueuePriorities = &queuePriority};

    vk::PhysicalDeviceFeatures deviceFeatures{};
    vk::DeviceCreateInfo createInfo{.queueCreateInfoCount = 1,
                                    .pQueueCreateInfos = &queueCreateInfo,
                                    .enabledExtensionCount = 0,
                                    .pEnabledFeatures = &deviceFeatures};

    if constexpr (enableValidationLayers)
    {
      createInfo.setEnabledLayerCount(validationLayers.size());
      createInfo.setPpEnabledLayerNames(validationLayers.data());
    }

    device_ = physicalDevice_.createDeviceUnique(createInfo);
    graphicsQueue_ = device_->getQueue(indices.value(), 0);
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

      if (indices.isComplete())
        break;

      ++i;
    }

    return indices;
  }

  bool isDeviceSuitable(vk::PhysicalDevice device)
  {
    auto indices = findQueueFamilies(device);
    return indices.isComplete();
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

    for (std::string_view layerName : validationLayers)
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
