#include <sqlite3.h>
#include <sstream>
#include "db.h"
#include "types.h"

Db::Db() {
  sqlite3_open("bus326.db", &db);
  setup();
}

Db::~Db() {
  sqlite3_close(db);
}

void Db::setup() {
  char *err;
  sqlite3_exec(db, "create table if not exists history(id integer primary key autoincrement, version integer);", 0, 0, &err);
  auto lastHistory = getLastHistory().value_or(History {0, 0});
  if (lastHistory.version < 1) {
    sqlite3_exec(db, "create table location(id integer primary key autoincrement, vehicle_id text, last_updated text, latitude real, longitude real);", 0, 0, &err);
    sqlite3_exec(db, "insert into history(version) values(1);", 0, 0, &err);
  }
  // if (lastHistory.version < 2) {
  //   sqlite3_exec(db, "insert into history(version) values(2);", 0, 0, &err);
  // }
}

std::optional<History> Db::getLastHistory() {
  sqlite3_stmt *stmt;
  std::optional<History> history;
  sqlite3_prepare_v2(db, "select * from history order by id desc limit 1;", -1, &stmt, 0);
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    history.emplace(History {
      sqlite3_column_int(stmt, 0),
      sqlite3_column_int(stmt, 1)
    });
  }
  sqlite3_finalize(stmt);
  return history;
}

int Db::addLocation(quicktype::Vehicle &vehicle) {
  char *err;
  std::stringstream ss;
  ss << "insert into location(vehicle_id, last_updated, latitude, longitude) values("
    << "'" << vehicle.vehicle_id << "',"
    << "'" << vehicle.last_updated << "',"
    << vehicle.coords.at(0) << ","
    << vehicle.coords.at(1) << ");";
  sqlite3_exec(db, ss.str().c_str(), 0, 0, &err);
  return sqlite3_last_insert_rowid(db);
}

std::vector<quicktype::Vehicle> Db::getLocations() {
  sqlite3_stmt *stmt;
  std::vector<quicktype::Vehicle> vehicles;
  sqlite3_prepare_v2(db, "select * from location;", -1, &stmt, 0);
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    vehicles.push_back(quicktype::Vehicle {
      "", // headsign
      "", // route_id
      "", // pattern_id
      (char*)sqlite3_column_text(stmt, 1), // vehicle_id
      0, // stops_passed
      (char*)sqlite3_column_text(stmt, 2), // last_update
      {
        sqlite3_column_double(stmt, 3),
        sqlite3_column_double(stmt, 4)
      } // coords
    });
  }
  sqlite3_finalize(stmt);
  return vehicles;
}
