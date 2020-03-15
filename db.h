#include <optional>
#include <sqlite3.h>
#include "domain/repository.h"

struct History {
  int id;
  int version;
};

class Db : public domain::Repository {
  private:
    sqlite3 *db;
    void setup();
    std::optional<History> getLastHistory();
  public:
    Db();
    ~Db();
    int addLocation(domain::Vehicle &vehicle);
    std::vector<domain::Vehicle> getLocations();
    std::vector<std::string> getReports();
};
