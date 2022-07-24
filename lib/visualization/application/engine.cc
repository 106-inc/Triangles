#include "engine.hh"

namespace visual
{

Engine::Engine() :
    name{"Sample Name"}
{
}

bool Engine::prepare(Platform &platform)
{
  platform_ = &platform;
  return true;
}

void Engine::finish()
{
}

bool Engine::resize(const uint32_t /*width*/, const uint32_t /*height*/)
{
  return true;
}

void Engine::inputEvent(const InputEvent &input_event)
{
}

void Engine::update(/* float delta_time */)
{
//   fps        = 1.0f / delta_time;
//   frame_time = delta_time * 1000.0f;
}

const std::string &Engine::getName() const
{
  return name_;
}

void Engine::setName(const std::string &name)
{
  name_ = name;
}

DebugInfo &Engine::getDebugInfo()
{
  return debug_info;
}

} // namespace visual

