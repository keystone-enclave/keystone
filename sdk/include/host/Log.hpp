//******************************************************************************
// Copyright (c) 2020, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct LogLabelTagType {};
struct LogTimeTagType {};

class Logger {
 public:
  Logger() = default;
  explicit Logger(const std::string& path) : os_{new std::ofstream{path}} {}

  /* Direct all logging to the file at PATH. Returns whether it was
     successful. */
  inline bool DirectToFile(const std::string& path);

  /* Direct all logging to STDOUT. Returns whether it was successful. */
  inline bool DirectToSTDOUT();

  /* Direct all logging to STDERR. Returns whether it was successful. */
  inline bool DirectToSTDERR();

  /* Output all logs to the specified destination (e.g., STDOUT or a file).
     All provided logs except LogDebug are enabled when initialized. */
  inline Logger& Enable();

  /* Prevent the outputting of all logs to the specified destination (e.g.,
     STDOUT or a file). All provided logs except LogDebug are enabled when
     initialized. */
  inline Logger& Disable();

  /* If you want to number your logs automatically, you can do so via this
     operator, e.g.,

     LogDebug << LogLabelTag << " A\n";
     LogDebug << LogLabelTag << "B\n";

     will output "[1]A\n[2]B\n".*/
  inline Logger& operator<<(LogLabelTagType tag) const;

  /* As with the label above, but using a time stamp (LogTimeStamp).
     ctime_s() adds a newline so this needs to be changed (TODO). */
  Logger& operator<<(LogTimeTagType tag) const;

  /* Wrapper around the ostream << operator. */
  template <typename T>
  Logger& operator<<(const T& to_write) const {
    if (enabled_) {
      *os_ << to_write;
    }
    return *this;
  }

 private:
  std::ostream* os_{&std::cout};
  mutable int label_count_{1};
  bool enabled_{false};

  bool ResetOutputStream_(std::ostream* replacement);
};

Logger LogDebug                       = Logger{}.Disable();
Logger LogInfo                        = Logger{}.Enable();
Logger LogWarn                        = Logger{}.Enable();
Logger LogError                       = Logger{}.Enable();
constexpr LogLabelTagType LogLabel    = {};
constexpr LogTimeTagType LogTimeStamp = {};

enum class FormatMethod { Pretty, JSON, Default };

template <typename T>
class Formattable {
 public:
  /* Inherit from this class, using the class as T and override this method
     to support formatted prints. It's recommended to account for the METHOD
     (see above). The formatted output should be written to OS. */
  virtual void Format(std::ostream& os, FormatMethod method) const = 0;
};

template <typename T, FormatMethod M>
class DoFormat {
 public:
  explicit DoFormat(const T& to_format) : formattable_{std::cref(to_format)} {}

  std::string ToString() const {
    std::ostringstream oss{};
    formattable_.get().Format(oss, M);
    return oss.str();
  }

  const T& GetFormattable() const { return formattable_.get(); }

 private:
  std::reference_wrapper<const T> formattable_;
};

template <typename T, FormatMethod M>
std::ostream&
operator<<(std::ostream& os, const DoFormat<T, M>& f) {
  f.GetFormattable().Format(os, M);
  return os;
}

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
