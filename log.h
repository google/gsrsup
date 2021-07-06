// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

// A basic logging system built with command-line utilities in mind.

#ifndef GSRSUP_LOG_H_
#define GSRSUP_LOG_H_

#include <limits>
#include <ostream>
#include <string>

#include <absl/strings/str_cat.h>
#include <absl/strings/string_view.h>
#include <stdlib.h>

namespace gsrsup {

// A logger object backed by a std::ostream (or by nothing, if the ostream is
// null).
//
// This class inherits the thread-safety properties of its underlying ostream.
class Log final {
 public:
  enum Level : int {
    kDebug = 0,
    kInfo = 1,
    kWarning = 2,
    kError = 3,
    kNeverPrint = std::numeric_limits<int>::max(),
  };

  explicit Log(absl::string_view program, Level minimum_level,
               std::ostream* out)
      : prefix_(program), minimum_level_(minimum_level), out_(out) {}

  explicit Log(absl::string_view program, Level minimum_level,
               std::ostream& out)
      : Log(program, minimum_level, &out) {}

  explicit Log(absl::string_view program, std::ostream* out)
      : Log(program, kInfo, out) {}

  explicit Log(absl::string_view program, std::ostream& out)
      : Log(program, &out) {}

  Log(const Log&) = default;
  Log& operator=(const Log&) = default;

  template <typename... Args>
  void Message(Level, Args...);

  template <typename... Args>
  void Debug(Args... args) {
    Message(kDebug, args...);
  }

  template <typename... Args>
  void Info(Args... args) {
    Message(kInfo, args...);
  }

  template <typename... Args>
  void Warning(Args... args) {
    Message(kWarning, args...);
  }

  template <typename... Args>
  void Error(Args... args) {
    Message(kError, args...);
  }

 private:
  std::string prefix_;
  Level minimum_level_;
  std::ostream* out_;
};

template <typename... Args>
void Log::Message(Level level, Args... args) {
  if (level < minimum_level_ || out_ == nullptr) {
    return;
  }

  absl::string_view level_str;
  switch (level) {
    case kDebug:
      level_str = "debug: ";
      break;
    case kWarning:
      level_str = "warning: ";
      break;
    case kError:
      level_str = "error: ";
      break;
    default:
      break;
  }
  std::string message = absl::StrCat(prefix_, ": ", level_str, args..., "\n");
  out_->write(message.data(), message.size());
}

// If condition is false, logs an internal error and exits the program.
void Check(Log& log, bool condition, const char* condition_str,
           const char* file, int line);

}  // namespace gsrsup

// A convenience macro to invoke the Check function.
#define CHECK(log, condition) \
  ::gsrsup::Check((log), (condition), #condition, __FILE__, __LINE__)

#endif  // GSRSUP_LOG_H_
