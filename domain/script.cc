#include <iostream>
#include <thread>
#include <chrono>
#include "script.h"

namespace domain {
  void Script::run_loop() {
    Welcome payload = service.getPayload();

    for (auto location: payload.vehicle_locations) {
      for (auto vehicle: location.vehicles) {
        auto id = repository.addLocation(vehicle);
        std::cout << "write repository id=" << id << std::endl;
      }
    }
  }

  void Script::run() {
    for (auto i = 0; i < 1; i++) {
      if (i > 0) {
        std::cout << "sleep for 1m" << std::endl;
        std::this_thread::sleep_for(std::chrono::minutes(1));
      }
      std::cout << "nth try: " << i + 1 << std::endl;
      run_loop();
    }
  }
}