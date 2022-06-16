#include "engine.hh"

int main()
try
{
  Application app;
  app.run();
  return EXIT_SUCCESS;
}
catch (const vk::SystemError &e)
{
  std::cerr << "[VULKAN ERROR] " << e.what() << std::endl;
  return EXIT_FAILURE;
}
catch (const std::exception &e)
{
  std::cerr << "[APP ERROR] " << e.what() << std::endl;
  return EXIT_FAILURE;
}
catch (...)
{
  std::cerr << "[ERROR] Unkwnown error" << std::endl;
  return EXIT_FAILURE;
}
