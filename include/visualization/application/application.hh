#ifndef __INCLUDE_VISUALIZATION_APPLICATION_APPLICATION_HH__
#define __INCLUDE_VISUALIZATION_APPLICATION_APPLICATION_HH__

#include <memory>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "engine.hh"
#include "input.hh"
#include "window.hh"

namespace visual
{

enum class ExitCode
{
  Success = 0, /* App executed as expected */
  Help,        /* App should show help */
  Close,       /* App has been requested to close at initialization */
  FatalError   /* App encountered an unexpected error */
};

class Application
{
public:
  static const uint32_t MIN_WINDOW_WIDTH;
  static const uint32_t MIN_WINDOW_HEIGHT;

protected:
  std::unique_ptr<Window> window_{nullptr};
  std::unique_ptr<Engine> engine_{nullptr};

  Window::Properties windowProps_;
  bool processInputEvents_{true};
  bool closeRequested_{false};

private:
  // const EngineInfo * requestedEngine_{nullptr}; // TODO: Define EngineInfo class
  static std::string externalStorageDirectory_;
  static std::string temporaryDirectory_;

public:
  Application() = default;
  virtual ~Application() = default;

  virtual ExitCode init();

  ExitCode mainLoop();

  void update();

  virtual void terminate(/* ExitCode exitCode */);
  virtual void close();

  static const std::string &getExternalStorageDirectory(); // maybe use std::filesystem?
  static const std::string &getTemporaryDirectory();       // maybe use std::filesystem?

  virtual const char *getSurfaceExtension() = 0;

#if 0 // TODO: Define RenderContext and Device classes
  virtual std::unique_ptr<RenderContext> createRenderContext(
    Device &device, vk::SurfaceKHR surface,
    const std::vector<vk::SurfaceFormatKHR> &surfaceFormatPriority) const;
#endif

  virtual void resize(uint32_t width, uint32_t height);

  virtual void inputEvent(const InputEvent &inputEvent);

  Window &getWindow();

  Engine &getEngine() const;
  Engine &getEngine();

  static const std::string &setExternalStorageDirectory(); // maybe use std::filesystem?
  static const std::string &setTemporaryDirectory();       // maybe use std::filesystem?

  void requestEngine(/* const EngineInfo * engine */); // TODO: Define EngineInfo class
  bool engineRequested();
  bool startEngine();

  void disableInputProcessing();

  void setWindowProperties(/* const Window::OptionalProperties & properties */);
};

} // namespace visual

#endif // __INCLUDE_VISUALIZATION_APPLICATION_APPLICATION_HH__
