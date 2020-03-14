#include <iostream>
#include "json.hpp"
#include "httplib.h"
#include "db.h"
#include "mappers.h"
#include "domain/script.h"
#include "domain/service.h"

using nlohmann::json;
using nlohmann::from_json;
using nlohmann::to_json;

class Service : public domain::Service {
  private:
    httplib::SSLClient &client;
  public:
    Service(httplib::SSLClient &client) : client(client) {}
    inline domain::Welcome getPayload() {
      auto res = client.Get("/api/1/vehiclelocations?route=LondonBus326&region_id=uk-london");
      if (res->status != 200) {
        std::cout << "bad status=" << res->status << std::endl;
        throw 1; // Ugh!
      }
      auto payloadJson = json::parse(res->body);
      domain::Welcome payload;
      from_json(payloadJson, payload);
      return payload;
    }
};

int main() {
  httplib::SSLClient client {"citymapper.com"};
  Service service {client};
  Db db;

  domain::Script script {db, service};

  script.run();

  // debug dump
  auto vehicles = db.getLocations();
  for (auto vehicle: vehicles) {
    json j;
    to_json(j, vehicle);
    std::cout << j.dump(2) << std::endl;
  }
  return 0;
}
