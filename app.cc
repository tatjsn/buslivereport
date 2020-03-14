#include <iostream>
#include <thread>
#include <chrono>
#include "httplib.h"
#include "json.hpp"
#include "types.h"
#include "db.h"

using nlohmann::json;
using nlohmann::from_json;
using nlohmann::to_json;
using quicktype::Welcome;
using quicktype::Vehicle;

int run_loop(Db &db, httplib::Client &cli) {
  auto res = cli.Get("/api/1/vehiclelocations?route=LondonBus326&region_id=uk-london");
  if (res->status != 200) {
    std::cout << "bad status=" << res->status << std::endl;
    return 1;
  }
  std::cout << res->body << std::endl;

  auto payloadJson = json::parse(res->body);
  Welcome payload;
  from_json(payloadJson, payload);

  for (auto location: payload.vehicle_locations) {
    for (auto vehicle: location.vehicles) {
      auto id = db.addLocation(vehicle);
      std::cout << "write db id=" << id << std::endl;
    }
  }
  return 0;
}

int main() {
  Db db;
  auto host = "citymapper.com";

  httplib::SSLClient cli(host);

  for (auto i = 0; i < 10; i++) {
    if (i > 0) {
      std::cout << "sleep for 1m" << std::endl;
      std::this_thread::sleep_for(std::chrono::minutes(1));
    }
    std::cout << "nth try: " << i + 1 << std::endl;
    auto err = run_loop(db, cli);
    if (err != 0) {
      return err;
    }
  }

  auto vehicles = db.getLocations();
  for (auto vehicle: vehicles) {
    json j;
    to_json(j, vehicle);
    std::cout << j.dump(2) << std::endl;
  }

  return 0;
}
