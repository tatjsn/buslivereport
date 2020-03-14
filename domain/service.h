#pragma once

#include "types.h"

namespace domain {
  class Service {
    public:
      virtual Welcome getPayload() = 0;
  };
}
