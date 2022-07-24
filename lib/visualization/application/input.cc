#include "input.hh"

namespace visual
{

InputEvent::InputEvent(EventSource source) : source{source}
{}

EventSource InputEvent::getSource() const
{
  return source;
}

KeyInputEvent::KeyInputEvent(KeyCode code, KeyAction action)
  : InputEvent(EventSource::Keyboard), code_(code), action_(action)
{}

KeyCode KeyInputEvent::getCode() const
{
  return code;
}

KeyAction KeyInputEvent::getAction() const
{
  return action;
}

MouseButtonInputEvent::MouseButtonInputEvent(MouseButton button, MouseAction action, float posX,
                                             float posY)
  : InputEvent(EventSource::Mouse), button_(button), action_(action), posX_(posX), posY_(posY)
{}

MouseButton MouseButtonInputEvent::getButton() const
{
  return button_;
}

MouseAction MouseButtonInputEvent::getAction() const
{
  return action_;
}

float MouseButtonInputEvent::getPosX() const
{
  return posX_;
}

float MouseButtonInputEvent::getPosY() const
{
  return posY_;
}

} // namespace visual
