#include <optional>
#include <sqlite3.h>
#include "types.h"

struct History {
  int id;
  int version;
};

class Db {
  private:
    sqlite3 *db;
    void setup();
    std::optional<History> getLastHistory();
  public:
    Db();
    ~Db();
    int addLocation(quicktype::Vehicle &vehicle);
    std::vector<quicktype::Vehicle> getLocations();
};
