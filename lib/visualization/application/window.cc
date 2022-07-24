#include "window.hh"
#include "engine.hh"

namespace visual
{

Window::Window(const Properties &props) : properties_(props)
{}

void Window::processEvents()
{}

Window::Extent Window::resize(const Extent &newExtent)
{
  if (properties_.resizable)
  {
    properties_.extent.width = newExtent.width;
    properties_.extent.height = newExtent.height;
  }

  return propterties_.extent;
}

const Window::Extent &Window::getExtent() const
{
  return properties_.extent;
}

Window::Mode Window::getWindowMode() const
{
  return properties_.mode;
}

} // namespace visual
