//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------

#include "Log.hpp"

/* Direct all logging to the file at PATH. Returns whether it was
   successful. */
bool
Logger::DirectToFile(const std::string& path) {
  return ResetOutputStream_(new std::ofstream{path});
}

/* Direct all logging to STDOUT. Returns whether it was successful. */
bool
Logger::DirectToSTDOUT() {
  return ResetOutputStream_(&std::cout);
}

/* Direct all logging to STDERR. Returns whether it was successful. */
bool
Logger::DirectToSTDERR() {
  return ResetOutputStream_(&std::cerr);
}

/* Output all logs to the specified destination (e.g., STDOUT or a file).
   All provided logs except LogDebug are enabled when initialized. */
Logger&
Logger::Enable() {
  enabled_ = true;
  return *this;
}

/* Prevent the outputting of all logs to the specified destination (e.g.,
   STDOUT or a file). All provided logs except LogDebug are enabled when
   initialized. */
Logger&
Logger::Disable() {
  enabled_ = false;
  return *this;
}

/* If you want to number your logs automatically, you can do so via this
   operator, e.g.,

   LogDebug << LogLabelTag << " A\n";
   LogDebug << LogLabelTag << "B\n";

   will output "[1]A\n[2]B\n".*/
Logger&
Logger::operator<<(LogLabelTagType tag) const {
  if (enabled_) {
    *os_ << '[' << label_count_++ << ']';
  }
  return *this;
}

/* As with the label above, but using a time stamp (LogTimeStamp).
   ctime_s() adds a newline so this needs to be changed. */
Logger&
Logger::operator<<(LogTimeTagType tag) const {
  if (enabled_) {
    using clock_t    = std::chrono::system_clock;
    std::time_t time = clock_t::to_time_t(clock_t::now());
    char buf[26];
    ctime_s(buf, 26, &time);
    *os_ << '[' << buf << ']';
  }
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
