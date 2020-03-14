#include <iostream>
#include "httplib.h"
#include "json.hpp"
#include "types.h"
#include "db.h"

using nlohmann::json;
using nlohmann::from_json;
using nlohmann::to_json;
using quicktype::Welcome;
using quicktype::Vehicle;

int main() {
  Db db;
  auto host = "citymapper.com";
  auto port = 80;
  httplib::Client cli(host, port);
  cli.set_follow_location(true);
  auto res = cli.Get("/api/1/vehiclelocations?route=LondonBus326&region_id=uk-london");
  if (res->status != 200) {
    std::cout << "bad status=" << res->status << std::endl;
    return 1;
  }
  std::cout << "got data" << std::endl;
  std::cout << res->body << std::endl;

  auto payloadJson = json::parse(res->body);
  Welcome payload;
  from_json(payloadJson, payload);

  auto id = db.addLocation(payload.vehicle_locations.at(0).vehicles.at(0));

  std::cout << "write db id=" << id << std::endl;

  auto vehicles = db.getLocations();
  for (auto vehicle: vehicles) {
    json j;
    to_json(j, vehicle);
    std::cout << j.dump(2) << std::endl;
  }
  return 0;
}
