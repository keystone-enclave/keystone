#include "Log.hpp"

namespace Keystone {

bool
Logger::DirectToFile(const std::string& path) {
  return ResetOutputStream_(new std::ofstream{path});
}

bool
Logger::DirectToSTDOUT() {
  return ResetOutputStream_(&std::cout);
}

bool
Logger::DirectToSTDERR() {
  return ResetOutputStream_(&std::cerr);
}

Logger&
Logger::Enable() {
  enabled_ = true;
  return *this;
}

Logger&
Logger::Disable() {
  enabled_ = false;
  return *this;
}

bool
Logger::ResetOutputStream_(std::ostream* replacement) {
  if (!replacement || replacement->fail()) {
    return false;
  }
  if (os_ != &std::cout && os_ != &std::cerr) {
    dynamic_cast<std::ofstream*>(os_)->close();
    delete os_;
  }
  os_ = replacement;
  return true;
}
}  // namespace Keystone
