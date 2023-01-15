#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace Keystone {

class Logger {
 public:
  Logger() = default;
  ~Logger();

  inline bool DirectToFile(const std::string& path, bool append = false) {
    return ResetOutputStream_(new std::ofstream{
        path, append ? std::ios_base::app : std::ios_base::out});
  }

  inline bool DirectToSTDOUT() { return ResetOutputStream_(&std::cout); }

  inline bool DirectToSTDERR() { return ResetOutputStream_(&std::cerr); }

  inline Logger& Enable() {
    enabled_ = true;
    return *this;
  }

  inline Logger& Disable() {
    enabled_ = false;
    return *this;
  }

  template <typename T>
  inline const Logger& operator<<(T&& to_write) const {
    if (enabled_) {
      *os_ << std::forward<T>(to_write);
    }
    return *this;
  }

  inline void ForceWrite() { os_->flush(); }

 private:
  std::ostream* os_{&std::cout};
  bool enabled_{true};

  bool ResetOutputStream_(std::ostream* replacement);
};

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
