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

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

class Application
{
public:
  void run();

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

  void initWindow();
  static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
  void initVulkan();
  void mainLoop();
  void cleanup();
  void recreateSwapChain();
  void createInstance();
  void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo);
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createDescriptorSetLayout();
  void createGraphicsPipeline();
  void createFramebuffer();
  void createCommandPool();
  void createDepthResources();
  vk::Format findSupportedFormat(const std::vector<vk::Format> &candidates, vk::ImageTiling tiling,
                                 vk::FormatFeatureFlags features);
  vk::Format findDepthFormat();
  bool hasStencilComponent(vk::Format format);
  void createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
                   vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image &image,
                   vk::DeviceMemory &imageMemory);
  vk::ImageView createImageView(vk::Image image, vk::Format format,
                                vk::ImageAspectFlags aspectFlags);
  void createVertexBuffer();
  void createIndexBuffer();
  void createUniformBuffers();
  void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
                    vk::MemoryPropertyFlags properties, vk::Buffer &buffer,
                    vk::DeviceMemory &bufferMemory);
  void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);
  void createDescriptorPool();
  void createDescriptorSets();
  uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
  void createCommandBuffers();
  void recordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex);
  void createSyncObjects();
  void updateUniformBuffer(uint32_t currentImage);
  void drawFrame();
  vk::UniqueShaderModule createShaderModule(const std::vector<char> &code);
  vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR> &availableFormats);
  void createSwapChain();
  void createImageViews();
  void createRenderPass();
  vk::PresentModeKHR chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR> &availablePresentModes);
  vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities);
  QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);
  SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device);
  bool isDeviceSuitable(vk::PhysicalDevice device);
  bool checkDeviceExtensionSupport(vk::PhysicalDevice device);
  std::vector<const char *> getRequiredExtensions();
  bool checkValidationLayerSupport();
};
