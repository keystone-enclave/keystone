//******************************************************************************
// Copyright (c) 2020, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>

namespace Keystone {

class Logger {
 public:
  Logger() = default;
  Logger(bool enable) : enabled_{enable} {}
  Logger(const Logger&)            = delete;
  Logger& operator=(const Logger&) = delete;
  ~Logger();

  /* Directs all logs to the file at PATH. Returns whether it was successful.

    If APPEND is true, writes start at the end of the file. Otherwise, the file
    is cleared and written to from the start.

    Do NOT have multiple logs write to the same file as there will be
    synchronization issues. */
  inline bool DirectToFile(const std::string& path, bool append = false) {
    const std::lock_guard<std::mutex> lock{mtx_};
    return ResetOutputStream_(new std::ofstream{
        path, append ? std::ios_base::app : std::ios_base::out});
  }

  /* Direct all logs to STDOUT. Returns whether it was successful. */
  inline bool DirectToSTDOUT() {
    const std::lock_guard<std::mutex> lock{mtx_};
    return ResetOutputStream_(&std::cout);
  }

  /* Direct all logs to STDERR. Returns whether it was successful. */
  inline bool DirectToSTDERR() {
    const std::lock_guard<std::mutex> lock{mtx_};
    return ResetOutputStream_(&std::cerr);
  }

  /* Output all logs to the specified destination (e.g., STDOUT or a file).
     All provided logs except LogDebug are enabled when initialized. */
  inline Logger& Enable() {
    const std::lock_guard<std::mutex> lock{mtx_};
    enabled_ = true;
    return *this;
  }

  /* Prevent the outputting of all logs to the specified destination (e.g.,
   STDOUT or a file). All provided logs except LogDebug are enabled when
   initialized. */
  inline Logger& Disable() {
    const std::lock_guard<std::mutex> lock{mtx_};
    enabled_ = false;
    return *this;
  }

  /* Wrapper around the ostream << operator. */
  template <typename T>
  inline const Logger& operator<<(T&& to_write) const {
    const std::lock_guard<std::mutex> lock{mtx_};
    if (enabled_) {
      *os_ << std::forward<T>(to_write);
    }
    return *this;
  }

 private:
  mutable std::mutex mtx_{};
  std::ostream* os_{&std::cout};
  bool enabled_{true};

  bool ResetOutputStream_(std::ostream* replacement);

  inline void ForceWrite_() { os_->flush(); }
};

extern Logger LogDebug;
extern Logger LogInfo;
extern Logger LogWarn;
extern Logger LogError;

enum class FormatMethod { Pretty, JSON, Default };

template <typename T>
class Formattable {
 public:
  void Format(
      std::ostream& os, FormatMethod method = FormatMethod::Default) const {
    switch (method) {
      case FormatMethod::JSON:
      case FormatMethod::Default:
        FormatAsJSON(os);
        break;
      case FormatMethod::Pretty:
        FormatAsPretty(os);
    }
  }

  virtual void FormatAsJSON(std::ostream& os) const   = 0;
  virtual void FormatAsPretty(std::ostream& os) const = 0;
};

template <typename T, FormatMethod M>
class DoFormat {
 public:
  explicit DoFormat(const T& to_format) : formattable_{std::cref(to_format)} {}

  std::string ToString() const {
    std::ostringstream oss{};
    oss << *this;
    return oss.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const DoFormat<T, M>& f) {
    f.formattable_.get().Format(os, M);
    return os;
  }

 private:
  std::reference_wrapper<const T> formattable_;
};

/* Convenience helpers. */
template <typename T>
DoFormat<T, FormatMethod::JSON>
FormatAsJSON(const T& to_format) {
  return DoFormat<T, FormatMethod::JSON>{to_format};
}

template <typename T>
DoFormat<T, FormatMethod::Pretty>
FormatAsPretty(const T& to_format) {
  return DoFormat<T, FormatMethod::Pretty>{to_format};
}

template <typename T>
DoFormat<T, FormatMethod::Default>
FormatAsDefault(const T& to_format) {
  return DoFormat<T, FormatMethod::Default>{to_format};
}

template <typename T>
DoFormat<T, FormatMethod::Default>
Format(const T& to_format) {
  return DoFormat<T, FormatMethod::Default>{to_format};
}

}  // namespace Keystone
