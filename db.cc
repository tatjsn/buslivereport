#include <iostream>
#include <sqlite3.h>
#include <sstream>
#include "db.h"

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
    sqlite3_exec(db, "create table location(id integer primary key autoincrement, headsign text, pattern_id text, vehicle_id text, stops_passed real, last_updated text, latitude real, longitude real);", 0, 0, &err);
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

int Db::addLocation(domain::Vehicle &vehicle) {
  char *err;
  std::stringstream ss;
  ss << "insert into location(headsign, pattern_id, vehicle_id, stops_passed, last_updated, latitude, longitude) values("
    << "'" << vehicle.headsign << "',"
    << "'" << vehicle.pattern_id << "',"
    << "'" << vehicle.vehicle_id << "',"
    << vehicle.stops_passed << ","
    << "'" << vehicle.last_updated << "',"
    << vehicle.coords.at(0) << ","
    << vehicle.coords.at(1) << ");";
  sqlite3_exec(db, ss.str().c_str(), 0, 0, &err);
  return sqlite3_last_insert_rowid(db);
}

std::vector<domain::Vehicle> Db::getLocations() {
  sqlite3_stmt *stmt;
  std::vector<domain::Vehicle> vehicles;
  sqlite3_prepare_v2(db, "select * from location;", -1, &stmt, 0);
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    vehicles.push_back(domain::Vehicle {
      (char*)sqlite3_column_text(stmt, 1), // headsign
      "", // route_id
      (char*)sqlite3_column_text(stmt, 2), // pattern_id
      (char*)sqlite3_column_text(stmt, 3), // vehicle_id
      sqlite3_column_double(stmt, 4), // stops_passed
      (char*)sqlite3_column_text(stmt, 5), // last_update
      {
        sqlite3_column_double(stmt, 6),
        sqlite3_column_double(stmt, 7)
      } // coords
    });
  }
  sqlite3_finalize(stmt);
  return vehicles;
}

std::vector<std::string> Db::getReports() {
  sqlite3_stmt *stmt;
  std::vector<std::string> reports;
  sqlite3_prepare_v2(
    db,
    "select id, vehicle_id, cast(round(stops_passed) as int), datetime(last_updated), case when datetime(last_updated)=(select max(datetime(last_updated)) from location) then 'live' else '' end from location where vehicle_id in (select vehicle_id from location where datetime(last_updated) = (select max(datetime(last_updated)) from location) and cast(round(stops_passed) as int) <= 23 and pattern_id='61326_Y0552491_1') and cast(round(stops_passed) as int) <= 23 and pattern_id='61326_Y0552491_1' and datetime(last_updated) between datetime('now', '-30 minute') and datetime('now') order by vehicle_id;", -1, &stmt, 0);
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    std::stringstream ss;
    ss << sqlite3_column_int(stmt, 0) // id
    << "|" << (char*)sqlite3_column_text(stmt, 1) // vehicle_id
    << "|" << sqlite3_column_int(stmt, 2) // rsp
    << "|" << (char*)sqlite3_column_text(stmt, 3) // min(last_updated)
    << "|" << (char*)sqlite3_column_text(stmt, 4); // status
    reports.push_back(ss.str());
  }
  sqlite3_finalize(stmt);
  return reports;
}
