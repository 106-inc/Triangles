#ifndef __INCLUDE_VISUALIZATION_APPLICATION_ENGINE_HH__
#define __INCLUDE_VISUALIZATION_APPLICATION_ENGINE_HH__

#include "input.hh"

namespace visual
{

class Window;
class Application;

class Engine
{
public:
  Engine ();
  virtual ~Engine() = default;

  virtual bool prepare(Application &app);

  virtual void update(/* float deltaTime */);

  virtual void finish();

  virtual bool resize(const uint32_t width, const uint32_t height);

  virtual void inputEvent(const InputEvent &inputEvent);

  const std::string &getName() const;
  void setName(const std::string & name);

protected:
  Application *app_;

private:
  std::string name_;
};

} // namespace visual


#endif // __INCLUDE_VISUALIZATION_APPLICATION_ENGINE_HH__
