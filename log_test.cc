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

#include "log.h"

#include <iostream>
#include <sstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace gsrsup {
namespace {

using ::testing::HasSubstr;
using ::testing::IsEmpty;

TEST(LogTest, FormatsProgramAndMessage) {
  std::ostringstream s;
  Log("program", s).Message(Log::kInfo, "message");
  EXPECT_EQ(s.str(), "program: message\n");
}

TEST(LogTest, FormatsLevelDebug) {
  std::ostringstream s;
  Log("program", Log::kDebug, s).Message(Log::kDebug, "message");
  EXPECT_THAT(s.str(), HasSubstr("debug:"));
}

TEST(LogTest, FormatsLevelWarning) {
  std::ostringstream s;
  Log("program", s).Message(Log::kWarning, "message");
  EXPECT_THAT(s.str(), HasSubstr("warning:"));
}

TEST(LogTest, FormatsLevelError) {
  std::ostringstream s;
  Log("program", s).Message(Log::kError, "message");
  EXPECT_THAT(s.str(), HasSubstr("error:"));
}

TEST(LogTest, HidesUninterestingMessages) {
  std::ostringstream s;
  Log("program", Log::kError, s).Message(Log::kDebug, "some debug message");
  EXPECT_THAT(s.str(), IsEmpty());
}

TEST(LogTest, DedicatedDebugFunction) {
  std::ostringstream s;
  Log("program", Log::kDebug, s).Debug("message");
  EXPECT_THAT(s.str(), HasSubstr("debug:"));
}

TEST(LogTest, DedicatedInfoFunction) {
  std::ostringstream s;
  Log("program", s).Info("message");
  EXPECT_EQ(s.str(), "program: message\n");
}

TEST(LogTest, DedicatedWarningFunction) {
  std::ostringstream s;
  Log("program", s).Warning("message");
  EXPECT_THAT(s.str(), HasSubstr("warning:"));
}

TEST(LogTest, DedicatedErrorFunction) {
  std::ostringstream s;
  Log("program", s).Error("message");
  EXPECT_THAT(s.str(), HasSubstr("error:"));
}

}  // namespace
}  // namespace gsrsup
