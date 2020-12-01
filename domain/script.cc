#include <iostream>
#include <thread>
#include <chrono>
#include "script.h"

namespace domain {
  void Script::run_aggregate_single() {
    Welcome payload = service.getPayload();

    for (auto location: payload.vehicle_locations) {
      for (auto vehicle: location.vehicles) {
        auto id = repository.addLocation(vehicle);
        // std::cout << "write repository id=" << id << std::endl;
      }
    }
  }

  void Script::run_aggregate() {
    for (auto i = 0; i < 60; i++) {
      if (i > 0) {
        std::cout << "sleep for 1m" << std::endl;
        std::this_thread::sleep_for(std::chrono::minutes(1));
      }
      std::cout << "nth try: " << i + 1 << std::endl;
      run_aggregate_single();
    }
  }

  void Script::run_livereport() {
    for (auto i = 0; i < 60; i++) {
      if (i > 0) {
        std::cout << "sleep for 1m" << std::endl;
        std::this_thread::sleep_for(std::chrono::minutes(1));
      }
      std::cout << "\e[1;1H\e[2J"; // clear
      std::cout << "nth try: " << i + 1 << std::endl;
      run_aggregate_single();
      std::cout << "aggregate done" << std::endl;
      auto reports = repository.getReports();
      std::cout << "start report" << std::endl;
      for (auto report: reports) {
        std::cout << report << std::endl;
      }
      std::cout << "end report" << std::endl;
    }
  }
}