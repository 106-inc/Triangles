#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char *> VALIDATION_LAYERS{"VK_LAYER_KHRONOS_validation"};
const std::vector<const char *> DEVICE_EXTENSIONS{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

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

struct Vertex
{
  glm::vec3 pos;
  glm::vec3 color;

  static vk::VertexInputBindingDescription getBindingDescription()
  {
    vk::VertexInputBindingDescription bindingDescription{
      .binding = 0, .stride = sizeof(Vertex), .inputRate = vk::VertexInputRate::eVertex};
    return bindingDescription;
  }

  static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions()
  {
    std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions{
      vk::VertexInputAttributeDescription{.location = 0,
                                          .binding = 0,
                                          .format = vk::Format::eR32G32B32Sfloat,
                                          .offset = offsetof(Vertex, pos)},
      vk::VertexInputAttributeDescription{.location = 1,
                                          .binding = 0,
                                          .format = vk::Format::eR32G32B32Sfloat,
                                          .offset = offsetof(Vertex, color)}};

    return attributeDescriptions;
  }
};

struct UniformBufferObject
{
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

const std::vector<Vertex> VERTICES = {
  {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},  {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
  {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},

  {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
  {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},   {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}};

const std::vector<uint16_t> INDICES = {0, 1, 2, 2, 3, 0,

                                       4, 5, 6, 6, 7, 4};

static std::vector<char> readFile(const std::string &filename)
{
  std::ifstream file{filename, std::ios::ate | std::ios::binary};

  if (!file.is_open())
    throw std::runtime_error{"failed to open file!"};

  auto fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return buffer;
}

std::string toStringMessageSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT s)
{
  switch (s)
  {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    return "VERBOSE";
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    return "ERROR";
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    return "WARNING";
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    return "INFO";
  default:
    return "UNKNOWN";
  }
}
std::string toStringMessageType(VkDebugUtilsMessageTypeFlagsEXT s)
{
  if (s == 7)
    return "General | Validation | Performance";
  if (s == 6)
    return "Validation | Performance";
  if (s == 5)
    return "General | Performance";
  if (s == 4 /*VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT*/)
    return "Performance";
  if (s == 3)
    return "General | Validation";
  if (s == 2 /*VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT*/)
    return "Validation";
  if (s == 1 /*VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT*/)
    return "General";
  return "Unknown";
}

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
  auto ms = toStringMessageSeverity(messageSeverity);
  auto mt = toStringMessageType(messageType);

  std::cerr << "[" << ms << ": " << mt << "] " << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator,
                                      VkDebugUtilsMessengerEXT *pCallback)
{
  auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
    vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
  if (func != nullptr)
    return func(instance, pCreateInfo, pAllocator, pCallback);
  return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator)
{
  auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
    vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
  if (func != nullptr)
    func(instance, debugMessenger, pAllocator);
}

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
  using UniqueDebugUtilsMessengerEXTDynamic =
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>;

  GLFWwindow *window_;

  vk::UniqueInstance instance_;
  UniqueDebugUtilsMessengerEXTDynamic debugMessenger_;
  vk::DispatchLoaderDynamic dld_;
  vk::UniqueSurfaceKHR surface_;

  vk::PhysicalDevice physicalDevice_;
  vk::UniqueDevice device_;

  vk::Queue graphicsQueue_;
  vk::Queue presentQueue_;

  vk::UniqueSwapchainKHR swapChain_;
  std::vector<vk::Image> swapChainImages_;
  vk::Format swapChainImageFormat_;
  vk::Extent2D swapChainExtent_;
  std::vector<vk::UniqueImageView> swapChainImageViews_;
  std::vector<vk::UniqueFramebuffer> swapChainFramebuffers_;

  vk::UniqueRenderPass renderPass_;
  vk::UniqueDescriptorSetLayout descriptorSetLayout_;
  vk::UniquePipelineLayout pipelineLayout_;
  vk::UniquePipeline graphicsPipeline_;

  vk::UniqueCommandPool commandPool_;

  vk::Image depthImage_;
  vk::DeviceMemory depthImageMemory_;
  vk::ImageView depthImageView_;

  vk::Buffer vertexBuffer_;
  vk::DeviceMemory vertexBufferMemory_;

  vk::Buffer indexBuffer_;
  vk::DeviceMemory indexBufferMemory_;

  std::vector<vk::Buffer> uniformBuffers_;
  std::vector<vk::DeviceMemory> uniformBuffersMemory_;

  vk::UniqueDescriptorPool descriptorPool_;
  std::vector<vk::DescriptorSet> descriptorSets_;

  std::vector<vk::UniqueCommandBuffer> commandBuffers_;

  std::vector<vk::UniqueSemaphore> imageAvailableSemaphores_;
  std::vector<vk::UniqueSemaphore> renderFinishedSemaphores_;

  std::vector<vk::UniqueFence> inFlightFences_;

  uint32_t currentFrame_ = 0;
  bool framebufferResized_ = false;

  void initWindow()
  {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(WIDTH, HEIGHT, "Triangle", nullptr, nullptr);
  }

  static void framebufferResizeCallback(GLFWwindow *window, int width, int height)
  {
    auto app = reinterpret_cast<HelloTriangleApplication *>(glfwGetWindowUserPointer(window));
    app->framebufferResized_ = true;
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
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();
    // createDepthResources();
    createFramebuffer();
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();
  }

  void mainLoop()
  {
    while (!glfwWindowShouldClose(window_))
    {
      glfwPollEvents();
      drawFrame();
    }

    device_->waitIdle();
  }

  void cleanup()
  {
    glfwDestroyWindow(window_);
    glfwTerminate();

    device_->destroyBuffer(vertexBuffer_);
    device_->freeMemory(vertexBufferMemory_);

    device_->destroyBuffer(indexBuffer_);
    device_->freeMemory(indexBufferMemory_);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
      device_->destroyBuffer(uniformBuffers_[i]);
      device_->freeMemory(uniformBuffersMemory_[i]);
    }

    // device_->destroyImageView(depthImageView_);
    // device_->destroyImage(depthImage_);
    // device_->freeMemory(depthImageMemory_);
  }

  void recreateSwapChain()
  {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window_, &width, &height);
    while (width == 0 || height == 0)
    {
      glfwGetFramebufferSize(window_, &width, &height);
      glfwWaitEvents();
    }

    device_->waitIdle();

    // device_->destroyImageView(depthImageView_);
    // device_->destroyImage(depthImage_);
    // device_->freeMemory(depthImageMemory_);

    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    // createDepthResources();
    createFramebuffer();
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

    std::cout << "available extensions:" << std::endl;
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
    debugMessenger_ = instance_->createDebugUtilsMessengerEXTUnique(createInfo, nullptr, dld_);
  }

  void createSurface()
  {
    VkSurfaceKHR rawSurface;
    if (glfwCreateWindowSurface(*instance_, window_, nullptr, &rawSurface) != VK_SUCCESS)
      throw std::runtime_error("failed to create window surface!");

    surface_ = vk::UniqueSurfaceKHR{rawSurface, *instance_};
  }

  void pickPhysicalDevice()
  {
    auto devices = instance_->enumeratePhysicalDevices();
    if (0 == devices.size())
      throw std::runtime_error("failed to find GPUs with Vulkan support!");

    std::cout << "available devices:" << std::endl;
    for (const auto &device : devices)
      std::cout << '\t' << device.getProperties().deviceName << std::endl;

    for (const auto &device : devices)
      if (isDeviceSuitable(device))
      {
        std::cout << "chosen device: " << std::endl;
        std::cout << '\t' << device.getProperties().deviceName << std::endl;
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

  void createDescriptorSetLayout()
  {
    vk::DescriptorSetLayoutBinding uboLayoutBinding{.binding = 0,
                                                    .descriptorType =
                                                      vk::DescriptorType::eUniformBuffer,
                                                    .descriptorCount = 1,
                                                    .stageFlags = vk::ShaderStageFlagBits::eVertex,
                                                    .pImmutableSamplers = nullptr};

    vk::DescriptorSetLayoutCreateInfo layoutInfo{.bindingCount = 1, .pBindings = &uboLayoutBinding};
    descriptorSetLayout_ = device_->createDescriptorSetLayoutUnique(layoutInfo);
  }

  void createGraphicsPipeline()
  {
    auto vertShaderCode = readFile("shaders/vert.spv");
    auto fragShaderCode = readFile("shaders/frag.spv");

    auto vertShaderModule = createShaderModule(vertShaderCode);
    auto fragShaderModule = createShaderModule(fragShaderCode);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo{
      .stage = vk::ShaderStageFlagBits::eVertex, .module = *vertShaderModule, .pName = "main"};

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo{
      .stage = vk::ShaderStageFlagBits::eFragment, .module = *fragShaderModule, .pName = "main"};

    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
      .vertexBindingDescriptionCount = 1,
      .pVertexBindingDescriptions = &bindingDescription,
      .vertexAttributeDescriptionCount = attributeDescriptions.size(),
      .pVertexAttributeDescriptions = attributeDescriptions.data()};

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
      .topology = vk::PrimitiveTopology::eTriangleList, .primitiveRestartEnable = VK_FALSE};

    vk::Viewport viewport{.x = 0.0f,
                          .y = 0.0f,
                          .width = static_cast<float>(swapChainExtent_.width),
                          .height = static_cast<float>(swapChainExtent_.height),
                          .minDepth = 0.0f,
                          .maxDepth = 1.0f};

    vk::Rect2D scissor{.offset = {0, 0}, .extent = swapChainExtent_};

    vk::PipelineViewportStateCreateInfo viewportState{
      .viewportCount = 1, .pViewports = &viewport, .scissorCount = 1, .pScissors = &scissor};

    vk::PipelineRasterizationStateCreateInfo rasterizer{.depthClampEnable = VK_FALSE,
                                                        .rasterizerDiscardEnable = VK_FALSE,
                                                        .polygonMode = vk::PolygonMode::eFill,
                                                        .cullMode = vk::CullModeFlagBits::eBack,
                                                        .frontFace =
                                                          vk::FrontFace::eCounterClockwise,
                                                        .depthBiasEnable = VK_FALSE,
                                                        .depthBiasConstantFactor = 0.0f,
                                                        .depthBiasClamp = 0.0f,
                                                        .depthBiasSlopeFactor = 0.0f,
                                                        .lineWidth = 1.0f};

    vk::PipelineMultisampleStateCreateInfo multisampling{.rasterizationSamples =
                                                           vk::SampleCountFlagBits::e1,
                                                         .sampleShadingEnable = VK_FALSE,
                                                         .minSampleShading = 1.0f,
                                                         .pSampleMask = nullptr,
                                                         .alphaToCoverageEnable = VK_FALSE,
                                                         .alphaToOneEnable = VK_FALSE};

    // vk::PipelineDepthStencilStateCreateInfo depthStencil{.depthTestEnable = VK_TRUE,
    //                                                      .depthWriteEnable = VK_TRUE,
    //                                                      .depthCompareOp = vk::CompareOp::eLess,
    //                                                      .depthBoundsTestEnable = VK_FALSE,
    //                                                      .stencilTestEnable = VK_FALSE,
    //                                                      .front = {},
    //                                                      .back = {},
    //                                                      .minDepthBounds = 0.0f,
    //                                                      .maxDepthBounds = 1.0f};

    vk::PipelineColorBlendAttachmentState colorBlendAttachment{
      .blendEnable = VK_FALSE,
      .srcColorBlendFactor = vk::BlendFactor::eOne,
      .dstColorBlendFactor = vk::BlendFactor::eZero,
      .colorBlendOp = vk::BlendOp::eAdd,
      .srcAlphaBlendFactor = vk::BlendFactor::eOne,
      .dstAlphaBlendFactor = vk::BlendFactor::eZero,
      .alphaBlendOp = vk::BlendOp::eAdd,
      .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};

    vk::PipelineColorBlendStateCreateInfo colorBlending{
      .logicOpEnable = VK_FALSE,
      .logicOp = vk::LogicOp::eCopy,
      .attachmentCount = 1,
      .pAttachments = &colorBlendAttachment,
      .blendConstants = std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f}};

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo{.setLayoutCount = 1,
                                                    .pSetLayouts = &descriptorSetLayout_.get(),
                                                    .pushConstantRangeCount = 0,
                                                    .pPushConstantRanges = nullptr};

    pipelineLayout_ = device_->createPipelineLayoutUnique(pipelineLayoutInfo);

    vk::GraphicsPipelineCreateInfo pipelineInfo{.stageCount = 2,
                                                .pStages = shaderStages,
                                                .pVertexInputState = &vertexInputInfo,
                                                .pInputAssemblyState = &inputAssembly,
                                                .pViewportState = &viewportState,
                                                .pRasterizationState = &rasterizer,
                                                .pMultisampleState = &multisampling,
                                                .pDepthStencilState = nullptr /*&depthStencil*/,
                                                .pColorBlendState = &colorBlending,
                                                .pDynamicState = nullptr,
                                                .layout = *pipelineLayout_,
                                                .renderPass = *renderPass_,
                                                .subpass = 0,
                                                .basePipelineHandle = nullptr,
                                                .basePipelineIndex = -1};

    auto graphicsPipelinesResult = device_->createGraphicsPipelinesUnique(nullptr, pipelineInfo);
    if (graphicsPipelinesResult.result != vk::Result::eSuccess)
      throw std::runtime_error{"failed to create graphics pipeline!"};

    graphicsPipeline_ = std::move(graphicsPipelinesResult.value[0]);
  }

  void createFramebuffer()
  {
    auto swapChainImageViewsSize = swapChainImageViews_.size();
    swapChainFramebuffers_.resize(swapChainImageViewsSize);

    for (std::size_t i = 0; i < swapChainImageViewsSize; ++i)
    {
      std::array<vk::ImageView, 1 /*2*/> attachments = {
        *swapChainImageViews_[i] /*, depthImageView_*/};

      vk::FramebufferCreateInfo framebufferInfo{.renderPass = *renderPass_,
                                                .attachmentCount =
                                                  static_cast<uint32_t>(attachments.size()),
                                                .pAttachments = attachments.data(),
                                                .width = swapChainExtent_.width,
                                                .height = swapChainExtent_.height,
                                                .layers = 1};

      swapChainFramebuffers_[i] = device_->createFramebufferUnique(framebufferInfo);
    }
  }

  void createCommandPool()
  {
    auto queueFamiliIndices = findQueueFamilies(physicalDevice_);

    vk::CommandPoolCreateInfo poolInfo{.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                       .queueFamilyIndex =
                                         queueFamiliIndices.graphicsFamily.value()};

    commandPool_ = device_->createCommandPoolUnique(poolInfo);
  }

  void createDepthResources()
  {
    auto depthFormat = findDepthFormat();

    createImage(swapChainExtent_.width, swapChainExtent_.height, depthFormat,
                vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment,
                vk::MemoryPropertyFlagBits::eDeviceLocal, depthImage_, depthImageMemory_);

    depthImageView_ = createImageView(depthImage_, depthFormat, vk::ImageAspectFlagBits::eDepth);
  }

  vk::Format findSupportedFormat(const std::vector<vk::Format> &candidates, vk::ImageTiling tiling,
                                 vk::FormatFeatureFlags features)
  {
    for (const auto &format : candidates)
    {
      auto props = physicalDevice_.getFormatProperties(format);
      if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
        return format;
      if (tiling == vk::ImageTiling::eOptimal &&
          (props.optimalTilingFeatures & features) == features)
        return format;
    }

    throw std::runtime_error{"failed to find supported format!"};
  }

  vk::Format findDepthFormat()
  {
    return findSupportedFormat(
      {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
      vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
  }

  bool hasStencilComponent(vk::Format format)
  {
    return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
  }

  void createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
                   vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image,
                   vk::DeviceMemory &imageMemory)
  {
    vk::ImageCreateInfo imageInfo{.imageType = vk::ImageType::e2D,
                                  .format = format,
                                  .extent =
                                    vk::Extent3D{.width = width, .height = height, .depth = 1},
                                  .mipLevels = 1,
                                  .arrayLayers = 1u,
                                  .samples = vk::SampleCountFlagBits::e1,
                                  .tiling = tiling,
                                  .usage = usage,
                                  .sharingMode = vk::SharingMode::eExclusive,
                                  .initialLayout = vk::ImageLayout::eUndefined};

    image = device_->createImage(imageInfo);

    auto memRequirements = device_->getImageMemoryRequirements(image);

    vk::MemoryAllocateInfo allocInfo{.allocationSize = memRequirements.size,
                                     .memoryTypeIndex =
                                       findMemoryType(memRequirements.memoryTypeBits, properties)};

    imageMemory = device_->allocateMemory(allocInfo);
    device_->bindImageMemory(image, imageMemory, 0);
  }

  vk::ImageView createImageView(vk::Image image, vk::Format format,
                                vk::ImageAspectFlags aspectFlags)
  {
    vk::ImageViewCreateInfo viewInfo{.image = image,
                                     .viewType = vk::ImageViewType::e2D,
                                     .format = format,
                                     .subresourceRange =
                                       vk::ImageSubresourceRange{.aspectMask = aspectFlags,
                                                                 .baseMipLevel = 0,
                                                                 .levelCount = 1,
                                                                 .baseArrayLayer = 0,
                                                                 .layerCount = 1}};

    return device_->createImageView(viewInfo);
  }

  void createVertexBuffer()
  {
    vk::DeviceSize bufferSize = sizeof(VERTICES[0]) * VERTICES.size();

    vk::Buffer stagingBuffer{};
    vk::DeviceMemory stagingBufferMemory{};
    createBuffer(
      bufferSize, vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eVertexBuffer,
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
      stagingBuffer, stagingBufferMemory);

    void *data = device_->mapMemory(stagingBufferMemory, 0, bufferSize);
    memcpy(data, VERTICES.data(), bufferSize);
    device_->unmapMemory(stagingBufferMemory);

    createBuffer(bufferSize,
                 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                 vk::MemoryPropertyFlagBits::eDeviceLocal, vertexBuffer_, vertexBufferMemory_);
    copyBuffer(stagingBuffer, vertexBuffer_, bufferSize);

    device_->destroyBuffer(stagingBuffer);
    device_->freeMemory(stagingBufferMemory);
  }

  void createIndexBuffer()
  {
    vk::DeviceSize bufferSize = sizeof(INDICES[0]) * INDICES.size();

    vk::Buffer stagingBuffer{};
    vk::DeviceMemory stagingBufferMemory{};
    createBuffer(
      bufferSize, vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eIndexBuffer,
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
      stagingBuffer, stagingBufferMemory);

    void *data = device_->mapMemory(stagingBufferMemory, 0, bufferSize);
    memcpy(data, INDICES.data(), bufferSize);
    device_->unmapMemory(stagingBufferMemory);

    createBuffer(bufferSize,
                 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
                 vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer_, indexBufferMemory_);
    copyBuffer(stagingBuffer, indexBuffer_, bufferSize);

    device_->destroyBuffer(stagingBuffer);
    device_->freeMemory(stagingBufferMemory);
  }

  void createUniformBuffers()
  {
    vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers_.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory_.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
      createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer,
                   vk::MemoryPropertyFlagBits::eHostVisible |
                     vk::MemoryPropertyFlagBits::eHostCoherent,
                   uniformBuffers_[i], uniformBuffersMemory_[i]);
  }

  void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
                    vk::MemoryPropertyFlags properties, vk::Buffer &buffer,
                    vk::DeviceMemory &bufferMemory)
  {
    vk::BufferCreateInfo bufferInfo{
      .size = size, .usage = usage, .sharingMode = vk::SharingMode::eExclusive};
    buffer = device_->createBuffer(bufferInfo);

    auto memRequirements = device_->getBufferMemoryRequirements(buffer);

    vk::MemoryAllocateInfo allocInfo{.allocationSize = memRequirements.size,
                                     .memoryTypeIndex =
                                       findMemoryType(memRequirements.memoryTypeBits, properties)};

    bufferMemory = device_->allocateMemory(allocInfo);
    device_->bindBufferMemory(buffer, bufferMemory, 0);
  }

  void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
  {
    vk::CommandBufferAllocateInfo allocInfo{.commandPool = *commandPool_,
                                            .level = vk::CommandBufferLevel::ePrimary,
                                            .commandBufferCount = 1};

    auto commandBuffers = device_->allocateCommandBuffersUnique(allocInfo);

    vk::CommandBufferBeginInfo beginInfo{.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
    commandBuffers[0]->begin(beginInfo);

    std::array<vk::BufferCopy, 1> copyRegion = {
      vk::BufferCopy{.srcOffset = 0, .dstOffset = 0, .size = size}};
    commandBuffers[0]->copyBuffer(srcBuffer, dstBuffer, copyRegion);

    commandBuffers[0]->end();

    std::array<vk::SubmitInfo, 1> submitInfo = {
      vk::SubmitInfo{.commandBufferCount = 1, .pCommandBuffers = &commandBuffers[0].get()}};
    graphicsQueue_.submit(submitInfo);
    graphicsQueue_.waitIdle();
  }

  void createDescriptorPool()
  {
    vk::DescriptorPoolSize poolSize{.type = vk::DescriptorType::eUniformBuffer,
                                    .descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)};
    vk::DescriptorPoolCreateInfo poolInfo{.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
                                          .poolSizeCount = 1,
                                          .pPoolSizes = &poolSize};

    descriptorPool_ = device_->createDescriptorPoolUnique(poolInfo);
  }

  void createDescriptorSets()
  {
    std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, *descriptorSetLayout_);
    vk::DescriptorSetAllocateInfo allocInfo{.descriptorPool = *descriptorPool_,
                                            .descriptorSetCount =
                                              static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
                                            .pSetLayouts = layouts.data()};

    descriptorSets_.resize(MAX_FRAMES_IN_FLIGHT);
    descriptorSets_ = device_->allocateDescriptorSets(allocInfo);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
      vk::DescriptorBufferInfo bufferInfo{
        .buffer = uniformBuffers_[i], .offset = 0, .range = sizeof(UniformBufferObject)};

      vk::WriteDescriptorSet descriptorWrite{.dstSet = descriptorSets_[i],
                                             .dstBinding = 0,
                                             .dstArrayElement = 0,
                                             .descriptorCount = 1,
                                             .descriptorType = vk::DescriptorType::eUniformBuffer,
                                             .pImageInfo = nullptr,
                                             .pBufferInfo = &bufferInfo,
                                             .pTexelBufferView = nullptr};

      device_->updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
    }
  }

  uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
  {
    auto memProperties = physicalDevice_.getMemoryProperties();
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
      if ((typeFilter & (1 << i)) &&
          (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        return i;

    throw std::runtime_error{"failed to find suitable memory type!"};
  }

  void createCommandBuffers()
  {
    vk::CommandBufferAllocateInfo allocInfo{.commandPool = *commandPool_,
                                            .level = vk::CommandBufferLevel::ePrimary,
                                            .commandBufferCount = MAX_FRAMES_IN_FLIGHT};

    commandBuffers_ = device_->allocateCommandBuffersUnique(allocInfo);
  }

  void recordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
  {
    vk::CommandBufferBeginInfo beginInfo{.flags = {}, .pInheritanceInfo = nullptr};
    commandBuffer.begin(beginInfo);

    // std::array<vk::ClearValue, 2> clearValues{};
    // clearValues[0].color = vk::ClearColorValue{std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}};
    // clearValues[1].depthStencil = vk::ClearDepthStencilValue{1.0f, 0};

    vk::RenderPassBeginInfo renderPassInfo{
      .renderPass = *renderPass_,
      .framebuffer = *swapChainFramebuffers_[imageIndex],
      .renderArea = vk::Rect2D{.offset = {0, 0}, .extent = swapChainExtent_}/*,
      .clearValueCount = static_cast<uint32_t>(clearValues.size()),
      .pClearValues = clearValues.data()*/};

    vk::ClearValue clearColor = {std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}};
    renderPassInfo.setClearValues(clearColor);

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline_);
    std::array<vk::Buffer, 1> vertexBuffers = {vertexBuffer_};
    std::array<vk::DeviceSize, 1> offsets = {0};
    commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);
    commandBuffer.bindIndexBuffer(indexBuffer_, 0, vk::IndexType::eUint16);
    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pipelineLayout_, 0, 1,
                                     &descriptorSets_[currentFrame_], 0, nullptr);
    commandBuffer.drawIndexed(INDICES.size(), /* instanceCount = */ 1,
                              /* firstIndex = */ 0, 0, /* firstInstance = */ 0);

    commandBuffer.endRenderPass();
    commandBuffer.end();
  }

  void createSyncObjects()
  {
    vk::FenceCreateInfo fenceInfo{.flags = vk::FenceCreateFlagBits::eSignaled};
    for (std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
      imageAvailableSemaphores_.push_back(device_->createSemaphoreUnique({}));
      renderFinishedSemaphores_.push_back(device_->createSemaphoreUnique({}));

      inFlightFences_.push_back(device_->createFenceUnique(fenceInfo));
    }
  }

  void updateUniformBuffer(uint32_t currentImage)
  {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto time =
      std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model =
      glm::rotate(glm::mat4{1.0f}, time * glm::radians(90.0f), glm::vec3{0.0f, 0.0f, 1.0f});
    ubo.view = glm::lookAt(glm::vec3{2.0f, 2.0f, 2.0f}, glm::vec3{0.0f, 0.0f, 0.0f},
                           glm::vec3{0.0f, 0.0f, 1.0f});
    ubo.proj = glm::perspective(
      glm::radians(45.0f), swapChainExtent_.width / static_cast<float>(swapChainExtent_.height),
      0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    auto *data = device_->mapMemory(uniformBuffersMemory_[currentImage], 0, sizeof(ubo));
    std::memcpy(data, &ubo, sizeof(ubo));
    device_->unmapMemory(uniformBuffersMemory_[currentImage]);
  }

  void drawFrame()
  {
    if (device_->waitForFences(1, &inFlightFences_[currentFrame_].get(), VK_TRUE,
                               std::numeric_limits<uint64_t>::max()) != vk::Result::eSuccess)
      throw std::runtime_error{"error while waiting fence!"};

    auto acquireRes = device_->acquireNextImageKHR(
      *swapChain_, std::numeric_limits<uint64_t>::max(), *imageAvailableSemaphores_[currentFrame_]);
    if (auto res = acquireRes.result; res == vk::Result::eErrorOutOfDateKHR)
    {
      framebufferResized_ = false;
      return recreateSwapChain();
    }
    else if (res != vk::Result::eSuccess && res != vk::Result::eSuboptimalKHR)
      throw std::runtime_error{"failed to acquire swap chain image!"};
    auto imageIndex = acquireRes.value;

    updateUniformBuffer(currentFrame_);

    if (device_->resetFences(1, &inFlightFences_[currentFrame_].get()) != vk::Result::eSuccess)
      throw std::runtime_error{"error while reseting fence!"};

    commandBuffers_[currentFrame_]->reset();
    recordCommandBuffer(*commandBuffers_[currentFrame_], imageIndex);

    vk::Semaphore waitSemaphores[] = {*imageAvailableSemaphores_[currentFrame_]};
    vk::Semaphore signalSemaphores[] = {*renderFinishedSemaphores_[currentFrame_]};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

    vk::SubmitInfo submitInfo{.waitSemaphoreCount = 1,
                              .pWaitSemaphores = waitSemaphores,
                              .pWaitDstStageMask = waitStages,
                              .commandBufferCount = 1,
                              .pCommandBuffers = &commandBuffers_[currentFrame_].get(),
                              .signalSemaphoreCount = 1,
                              .pSignalSemaphores = signalSemaphores};

    if (graphicsQueue_.submit(1, &submitInfo, *inFlightFences_[currentFrame_]) !=
        vk::Result::eSuccess)
      throw std::runtime_error{"failed to submit draw command buffer!"};

    vk::SwapchainKHR swapChains[] = {*swapChain_};
    vk::PresentInfoKHR presentInfo{.waitSemaphoreCount = 1,
                                   .pWaitSemaphores = signalSemaphores,
                                   .swapchainCount = 1,
                                   .pSwapchains = swapChains,
                                   .pImageIndices = &imageIndex,
                                   .pResults = nullptr};

    auto res = presentQueue_.presentKHR(presentInfo);
    if (res == vk::Result::eErrorOutOfDateKHR || res == vk::Result::eSuboptimalKHR)
      recreateSwapChain();
    else if (res != vk::Result::eSuccess)
      throw std::runtime_error{"failed to present"};

    currentFrame_ = (currentFrame_ + 1) % MAX_FRAMES_IN_FLIGHT;
  }

  vk::UniqueShaderModule createShaderModule(const std::vector<char> &code)
  {
    vk::ShaderModuleCreateInfo createInfo{.codeSize = code.size(),
                                          .pCode = reinterpret_cast<const uint32_t *>(code.data())};

    return device_->createShaderModuleUnique(createInfo);
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

    vk::SwapchainCreateInfoKHR createInfo{.surface = *surface_,
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

    swapChain_ = device_->createSwapchainKHRUnique(createInfo);
    swapChainImages_ = device_->getSwapchainImagesKHR(*swapChain_);
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

  void createRenderPass()
  {
    vk::AttachmentDescription colorAttachment{.format = swapChainImageFormat_,
                                              .samples = vk::SampleCountFlagBits::e1,
                                              .loadOp = vk::AttachmentLoadOp::eClear,
                                              .storeOp = vk::AttachmentStoreOp::eStore,
                                              .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
                                              .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
                                              .initialLayout = vk::ImageLayout::eUndefined,
                                              .finalLayout = vk::ImageLayout::ePresentSrcKHR};

    // vk::AttachmentDescription depthAttachment{.format = findDepthFormat(),
    //                                           .samples = vk::SampleCountFlagBits::e1,
    //                                           .loadOp = vk::AttachmentLoadOp::eClear,
    //                                           .storeOp = vk::AttachmentStoreOp::eDontCare,
    //                                           .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
    //                                           .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
    //                                           .initialLayout = vk::ImageLayout::eUndefined,
    //                                           .finalLayout =
    //                                             vk::ImageLayout::eDepthStencilAttachmentOptimal};

    vk::AttachmentReference colorAttachmentRef{.attachment = 0,
                                               .layout = vk::ImageLayout::eColorAttachmentOptimal};

    // vk::AttachmentReference depthAttachmentRef{
    //   .attachment = 1, .layout = vk::ImageLayout::eDepthStencilAttachmentOptimal};

    vk::SubpassDescription subpass{.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
                                   .colorAttachmentCount = 1,
                                   .pColorAttachments = &colorAttachmentRef/*,
                                   .pDepthStencilAttachment = &depthAttachmentRef*/};

    vk::SubpassDependency dependency{
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput/* |
                      vk::PipelineStageFlagBits::eEarlyFragmentTests*/,
      .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput/* |
                      vk::PipelineStageFlagBits::eEarlyFragmentTests*/,
      .srcAccessMask = {},
      .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite /*|
                       vk::AccessFlagBits::eDepthStencilAttachmentWrite*/};

    std::array<vk::AttachmentDescription, 1 /*2*/> attachments = {
      colorAttachment /*, depthAttachment*/};

    vk::RenderPassCreateInfo renderPassInfo{.attachmentCount =
                                              static_cast<uint32_t>(attachments.size()),
                                            .pAttachments = attachments.data(),
                                            .subpassCount = 1,
                                            .pSubpasses = &subpass,
                                            .dependencyCount = 1,
                                            .pDependencies = &dependency};

    renderPass_ = device_->createRenderPassUnique(renderPassInfo);
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

      if (device.getSurfaceSupportKHR(i, *surface_))
        indices.presentFamily = i;

      if (indices.isComplete())
        break;

      ++i;
    }

    return indices;
  }

  SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device)
  {
    SwapChainSupportDetails details{.capabilities = device.getSurfaceCapabilitiesKHR(*surface_),
                                    .formats = device.getSurfaceFormatsKHR(*surface_),
                                    .presentModes = device.getSurfacePresentModesKHR(*surface_)};

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

int main()
try
{
  HelloTriangleApplication app;
  app.run();
  return EXIT_SUCCESS;
}
catch (const vk::SystemError &e)
{
  std::cerr << "[VULKAN ERROR] " << e.what() << std::endl;
  return EXIT_FAILURE;
}
catch (const std::exception &e)
{
  std::cerr << "[APP ERROR] " << e.what() << std::endl;
  return EXIT_FAILURE;
}
catch (...)
{
  std::cerr << "[ERROR] Unkwnown error" << std::endl;
  return EXIT_FAILURE;
}
