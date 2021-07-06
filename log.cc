#include "log.h"

#include <absl/strings/substitute.h>
#include <stdlib.h>

namespace gsrsup {

void Check(Log& log, bool condition, const char* condition_str,
           const char* file, int line) {
  if (!condition) {
    log.Error(absl::Substitute("internal error ($0:$1): ", file, line),
              condition_str);
    exit(1);
  }
}

}  // namespace gsrsup
