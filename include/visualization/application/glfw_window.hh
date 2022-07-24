#ifndef __INCLUDE_VISUALIZATION_APPLICATION_GLFW_WINDOW_HH__
#define __INCLUDE_VISUALIZATION_APPLICATION_GLFW_WINDOW_HH__

#include "window.hh"

struct GLFWwindow;

namespace visual
{
class Application;

class GLFWWindow : public IWindow
{
private:
  GLFWwindow *handle = nullptr;

public:
  GLFWWindow(Application *platform, const IWindow::Properties &properties);
  virtual ~GLFWWindow();

  // VkSurfaceKHR createSurface(Instance &instance) override; // TODO: Define Instance class
  vk::SurfaceKHR createSurface(vk::Instance instance, vk::PhysicalDevice physicalDevice) override;

  void processEvents() override;

  bool shouldClose() override;
  void close() override;
};

} // namespace visual

#endif // __INCLUDE_VISUALIZATION_APPLICATION_GLFW_WINDOW_HH__
