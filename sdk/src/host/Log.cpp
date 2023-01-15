#include "Log.hpp"

namespace Keystone {

/* Close and free the ofstream if applicable. */
static void
DestroyIfFile(std::ostream* os) {
  if (os != &std::cout && os != &std::cerr) {
    dynamic_cast<std::ofstream*>(os)->close();
    delete os;
  }
}

Logger::~Logger() {
  ForceWrite();
  DestroyIfFile(os_);
}

bool
Logger::ResetOutputStream_(std::ostream* replacement) {
  if (!replacement) {
    return false;
  }

  if (replacement->fail()) {
    DestroyIfFile(replacement);
    return false;
  }

  ForceWrite();
  DestroyIfFile(os_);
  os_ = replacement;
  return true;
}

Logger LogDebug = Logger{}.Disable();
Logger LogInfo  = Logger{}.Enable();
Logger LogWarn  = Logger{}.Enable();
Logger LogError = Logger{}.Enable();

}  // namespace Keystone
