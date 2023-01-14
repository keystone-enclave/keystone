#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace Keystone {

class Logger {
 public:
  Logger() = default;

  inline bool DirectToFile(const std::string& path);

  inline bool DirectToSTDOUT();

  inline bool DirectToSTDERR();

  inline Logger& Enable();

  inline Logger& Disable();

  template <typename T>
  const Logger& operator<<(T&& to_write) const {
    if (enabled_ && os_) {
      *os_ << std::forward<T>(to_write);
    }
    return *this;
  }

 private:
  std::ostream* os_{&std::cout};
  bool enabled_{false};

  bool ResetOutputStream_(std::ostream* replacement);
};

Logger LogDebug = Logger{}.Disable();
Logger LogInfo  = Logger{}.Enable();
Logger LogWarn  = Logger{}.Enable();
Logger LogError = Logger{}.Enable();

}  // namespace Keystone
