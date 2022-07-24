#include "window.hh"
#include "engine.hh"

namespace visual
{

IWindow::IWindow(const Properties &props) : properties_(props)
{}

void IWindow::processEvents()
{}

IWindow::Extent IWindow::resize(const Extent &newExtent)
{
  if (properties_.resizable)
  {
    properties_.extent.width = newExtent.width;
    properties_.extent.height = newExtent.height;
  }

  return propterties_.extent;
}

const IWindow::Extent &IWindow::getExtent() const
{
  return properties_.extent;
}

IWindow::Mode IWindow::getWindowMode() const
{
  return properties_.mode;
}

} // namespace visual
