#include "application.hh"

namespace visual
{

const uint32_t Application::MIN_WINDOW_WIDTH = 420;
const uint32_t Application::MIN_WINDOW_HEIGHT = 320;

std::string Application::externalStorageDirectory = "";
std::string Application::tempDirectory = "";

virtual ExitCode Application::init()
{
  if (closeRequested_)
    return ExitCode::Close;

  createWindow(windowProperties_);
  if (!window_)
    return ExitCode::FatalError;

  return ExitCode::Success;
}

ExitCode Application::mainLoop()
{
  if (!engineRequested())
    return ExitCode::Close;

  while (!window->shouldClose() && !closeRequested_)
  {
    try
    {
      if (engineRequested())
      {
        if (!startEngine())
          return ExitCode::FatalError;

        engine_->update();
      }

      update();
      window_->processEvents();
    }
    catch (std::exception &e)
    {
      return ExitCode::FatalError();
    }
  }

  return ExitCode::Success;
}

void Application::update()
{
  /* ??? */
}

#if 0 // TODO: Define RenderContext and Device classes
virtual std::unique_ptr<RenderContext> Application::createRenderContext(
  Device &device, vk::SurfaceKHR surface,
  const std::vector<vk::SurfaceFormatKHR> &surfaceFormatPriority) const
{

}
#endif

void Application::terminate()
{
  if (engine_)
  {
    onEngineClose(engine_->getName());
    engine_->finish();
  }

  engine_->reset();
  window_.reset();

  onApplicationClose();
}

void Application::close()
{
  if (window_)
    window_->close();

  closeRequested_ = true;
}

void Application::disableInputProcessing()
{
  processInputEvents_ = false;
}

void Application::setWindowProperties(const Window::OptionalProperties &props)
{
  /*...*/
}

const std::string &Application::getExternalStorageDirectory()
{
  return externalStorageDirectory_;
}

const std::string &Application::getTemporaryDirectory()
{
  return temporaryDirectory_;
}

Engine &Application::getApp()
{
  assert(engine_ && "Engine is not valid");
  return *engine_;
}

Engine &Application::getApp() const
{
  assert(engine_ && "Engine is not valid");
  return *engine_;
}

Window &Application::getWindow()
{
  return *window_;
}

void Application::setExternalStorageDirectory(const std::string &dir)
{
  externalStorageDirectory_ = dir;
}

void Application::setTemporaryDirectory(const std::string &dir)
{
  temporaryDirectory_ = dir;
}

bool Application::engineRequested()
{
  return requestedEngine_ != nullptr;
}

void Application::requestApplication(const EngineInfo *engine)
{
  requestedEngine_ = engine;
}

bool Application::startEngine()
{
  auto *requestedEngineInfo = requestedEngine_;
  requestedEngine_ = nullptr;

  if (engine_)
  {
    // auto executionTime = timer_.stop();
    auto engineID = engine_->getName();
    engine_->finish();
  }

  engine_ = requestedEngineInfo->create();
  engine_->setName(requestedEnfineInfo->id);

  if (!engine_)
    return false;

  if (!engine_->prepare(*this))
    return false;

  onEngineStart(requestedEngineInfo->id);
  return true;
}

void Application::inputEvent(const InputEvent &inputEvent)
{
  if (processInputEvents_ && engine_)
    engine_->inputEvent(inputEvent);

  if (inputEvent.getSource() == EventSource::Keyboard)
  {
    const auto &keyEvent = static_cast<const KeyInputEvent &>(inputEvent);

    if (keyEvent.getCode() == KeyCode::Back || keyEvent.getCode() == KeyCode::Escape)
      close();
  }
}

void Application::resize(uint32_t width, uint32_t height)
{
  Window::Extent extent{std::max<uint32_t>(width, MIN_WINDOW_WIDTH),
                        std::max<uint32_t>(height, MIN_WINDOW_HEIGHT)};

  if (window_)
  {
    auto actualExtent = window_->resize(extent);
    if (engine_)
      engine_->resize(actualExtent.width, actualExtent.height);
  }
}

} // namespace visual
