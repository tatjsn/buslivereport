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
      auto res = client.Get(
        "/api/1/vehiclelocations"
        "?route=LondonBus326"
        "&region_id=uk-london");
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

void run_aggregate() {
  Db db;
  httplib::SSLClient client {"citymapper.com"};
  Service service {client};

  domain::Script script {db, service};

  script.run_aggregate();
}

void run_livereport() {
  Db db;
  httplib::SSLClient client {"citymapper.com"};
  Service service {client};

  domain::Script script {db, service};

  script.run_livereport();
}

void run_dump() {
  Db db;
  auto vehicles = db.getLocations();
  for (auto vehicle: vehicles) {
    json j;
    to_json(j, vehicle);
    std::cout << j.dump(2) << std::endl;
  }
}

int main(int argc, char* argv[]) {
  std::string aggregate = "aggregate";
  std::string livereport = "livereport";
  std::string dump = "dump";

  if (argc == 2 && aggregate.compare(argv[1]) == 0) {
    run_aggregate();
  } else if (argc == 2 && livereport.compare(argv[1]) == 0) {
    run_livereport();
  } else if (argc == 2 && dump.compare(argv[1]) == 0) {
    run_dump();
  }
  std::cout << "invalid argument" << std::endl;
  return 0;
}
