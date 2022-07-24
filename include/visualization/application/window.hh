#ifndef __INCLUDE_VISUALIZATION_APPLICATION_WINDOW_HH__
#define __INCLUDE_VISUALIZATION_APPLICATION_WINDOW_HH__

#include <cstdint>
#include <optional>
#include <string>

#include <vulkan/vulkan.hpp>

namespace visual
{

class IWindow
{
public:
  struct Extent
  {
    uint32_t width;
    uint32_t height;
  };

  struct OptionalExtent
  {
    std::optional<uint32_t> width;
    std::optional<uint32_t> height;
  };

  enum class Mode
  {
    Headless,
    Fullscreen,
    FullscreenBorderless,
    Default
  };

  enum class Vsync
  {
    OFF,
    ON,
    Default
  };

  struct OptionalProperties
  {
    std::optional<std::string> title;
    std::optional<Mode> mode;
    std::optional<bool> resizable;
    std::optional<Vsync> vsync;
    OptionalExtent extent;
  };

  struct Properties
  {
    std::string title = "";
    Mode mode = Mode::Default;
    bool resizable = true;
    Vsync vsync = Vsync::Default;
    Extent extent = {1280, 720};
  };

protected:
  Properties properties_;

public:
  IWindow(const Properties &props);
  virtual ~IWindow() = default;

  // virtual vk::SurfaceKHR createSurface(Instance &instance) = 0; // TODO: Define Instance class
  virtual vk::SurfaceKHR createSurface(vk::Instance instance, vk::PhysicalDevice physicalDevice) = 0;

  virtual bool shouldClose() = 0;
  virtual void processEvents();
  virtual void close() = 0;

  Extent resize(const Extent &extent);
  const Extent & getExtent() const;
  Mode getWindowMode() const;
};

} // namespace visual

#endif // __INCLUDE_VISUALIZATION_APPLICATION_WINDOW_HH__
