#pragma once

#include "types.h"

namespace domain {
  class Repository {
    public:
      virtual int addLocation(Vehicle &vehicle) = 0;
      virtual std::vector<Vehicle> getLocations() = 0;
  };
}
