//******************************************************************************
// Copyright (c) 2020, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
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
  ForceWrite_();
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

  ForceWrite_();
  DestroyIfFile(os_);
  os_ = replacement;
  return true;
}

Logger LogDebug{false};
Logger LogInfo{};
Logger LogWarn{};
Logger LogError{};

}  // namespace Keystone
