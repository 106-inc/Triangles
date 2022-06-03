#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
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

struct SwapChainSupportDetails
{
  vk::SurfaceCapabilitiesKHR capabilities;
  std::vector<vk::SurfaceFormatKHR> formats;
  std::vector<vk::PresentModeKHR> presentModes;
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
  vk::DispatchLoaderDynamic dld_;
  vk::SurfaceKHR surface_; // TODO: Figure out how to use unique version

  vk::PhysicalDevice physicalDevice_;
  vk::UniqueDevice device_;

  vk::Queue graphicsQueue_;
  vk::Queue presentQueue_;

  vk::SwapchainKHR swapChain_; // Not unique because it has to be destroyed before surface
  std::vector<vk::Image> swapChainImages_;
  vk::Format swapChainImageFormat_;
  vk::Extent2D swapChainExtent_;
  std::vector<vk::UniqueImageView> swapChainImageViews_;

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
    createSwapChain();
    createImageViews();
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
    device_->destroySwapchainKHR(swapChain_);
    instance_->destroySurfaceKHR(surface_);

    if constexpr (enableValidationLayers)
      instance_->destroyDebugUtilsMessengerEXT(debugMessenger_, nullptr, dld_);

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
      vk::StructureChain structureChain{createInfo, debugCreateInfo};
    }

    instance_ = vk::createInstanceUnique(createInfo);
    dld_ = {*instance_, vkGetInstanceProcAddr};

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

    vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);
    debugMessenger_ = instance_->createDebugUtilsMessengerEXT(createInfo, nullptr, dld_);
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
    vk::DeviceCreateInfo createInfo{
      .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
      .pQueueCreateInfos = queueCreateInfos.data(),
      .enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size()),
      .ppEnabledExtensionNames = DEVICE_EXTENSIONS.data(),
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

  vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR> &availableFormats)
  {
    for (const auto &availableFormat : availableFormats)
      if (vk::Format::eB8G8R8A8Srgb == availableFormat.format &&
          vk::ColorSpaceKHR::eSrgbNonlinear == availableFormat.colorSpace)
        return availableFormat;

    return availableFormats.front();
  }

  void createSwapChain()
  {
    auto swapChainSupport = querySwapChainSupport(physicalDevice_);

    auto surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    auto presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    auto extent = chooseSwapExtent(swapChainSupport.capabilities);

    auto imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        swapChainSupport.capabilities.maxImageCount < imageCount)
      imageCount = swapChainSupport.capabilities.maxImageCount;

    vk::SwapchainCreateInfoKHR createInfo{.surface = surface_,
                                          .minImageCount = imageCount,
                                          .imageFormat = surfaceFormat.format,
                                          .imageColorSpace = surfaceFormat.colorSpace,
                                          .imageExtent = extent,
                                          .imageArrayLayers = 1,
                                          .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
                                          .preTransform =
                                            swapChainSupport.capabilities.currentTransform,
                                          .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                          .presentMode = presentMode,
                                          .clipped = VK_TRUE};

    auto indices = findQueueFamilies(physicalDevice_);
    std::array<uint32_t, 2> queueFamilyIndices{indices.graphicsFamily.value(),
                                               indices.presentFamily.value()};

    if (indices.graphicsFamily == indices.presentFamily)
      createInfo.setImageSharingMode(vk::SharingMode::eExclusive);
    else
    {
      createInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
      createInfo.setQueueFamilyIndices(queueFamilyIndices);
    }

    swapChain_ = device_->createSwapchainKHR(createInfo);
    swapChainImages_ = device_->getSwapchainImagesKHR(swapChain_);
    swapChainImageFormat_ = surfaceFormat.format;
    swapChainExtent_ = extent;
  }

  void createImageViews()
  {
    auto swapChainImageSize = swapChainImages_.size();
    swapChainImageViews_.resize(swapChainImageSize);

    for (std::size_t i = 0; i < swapChainImageSize; ++i)
    {
      vk::ComponentMapping components{.r = vk::ComponentSwizzle::eIdentity,
                                      .g = vk::ComponentSwizzle::eIdentity,
                                      .b = vk::ComponentSwizzle::eIdentity,
                                      .a = vk::ComponentSwizzle::eIdentity};

      vk::ImageSubresourceRange imageSubresourceRange{.aspectMask = vk::ImageAspectFlagBits::eColor,
                                                      .baseMipLevel = 0,
                                                      .levelCount = 1,
                                                      .baseArrayLayer = 0,
                                                      .layerCount = 1};

      vk::ImageViewCreateInfo createInfo{.image = swapChainImages_[i],
                                         .viewType = vk::ImageViewType::e2D,
                                         .format = swapChainImageFormat_,
                                         .components = components,
                                         .subresourceRange = imageSubresourceRange};

      swapChainImageViews_[i] = device_->createImageViewUnique(createInfo);
    }
  }

  vk::PresentModeKHR chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR> &availablePresentModes)
  {
    for (const vk::PresentModeKHR &availablePresentMode : availablePresentModes)
      if (vk::PresentModeKHR::eMailbox == availablePresentMode)
        return availablePresentMode;

    return vk::PresentModeKHR::eFifo;
  }

  vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities)
  {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
      return capabilities.currentExtent;

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window_, &width, &height);

    vk::Extent2D actualExtent{.width = static_cast<uint32_t>(width),
                              .height = static_cast<uint32_t>(height)};

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                    capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                                     capabilities.maxImageExtent.height);

    return actualExtent;
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
        indices.presentFamily = i;

      if (indices.isComplete())
        break;

      ++i;
    }

    return indices;
  }

  SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device)
  {
    SwapChainSupportDetails details{.capabilities = device.getSurfaceCapabilitiesKHR(surface_),
                                    .formats = device.getSurfaceFormatsKHR(surface_),
                                    .presentModes = device.getSurfacePresentModesKHR(surface_)};

    return details;
  }

  bool isDeviceSuitable(vk::PhysicalDevice device)
  {
    auto indices = findQueueFamilies(device);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
      auto swapChainSupportDetails = querySwapChainSupport(device);
      swapChainAdequate =
        !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
  }

  bool checkDeviceExtensionSupport(vk::PhysicalDevice device)
  {
    auto availableExtrnsions = device.enumerateDeviceExtensionProperties();
    std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());

    for (const auto &extension : availableExtrnsions)
      requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
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
  std::cerr << "[validation layer] " << pCallbackData->pMessage << std::endl;
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
  std::cerr << "[ERROR] " << e.what() << std::endl;
  return EXIT_FAILURE;
}
catch (...)
{
  std::cerr << "[ERROR] Unkwnown error" << std::endl;
  return EXIT_FAILURE;
}
