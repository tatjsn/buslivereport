//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Welcome data = nlohmann::json::parse(jsonString);

#pragma once

#include "json.hpp"
#include "domain/types.h"

namespace nlohmann {
    void from_json(const json & j, domain::Route & x);
    void to_json(json & j, const domain::Route & x);

    void from_json(const json & j, domain::Vehicle & x);
    void to_json(json & j, const domain::Vehicle & x);

    void from_json(const json & j, domain::VehicleLocation & x);
    void to_json(json & j, const domain::VehicleLocation & x);

    void from_json(const json & j, domain::Welcome & x);
    void to_json(json & j, const domain::Welcome & x);

    inline void from_json(const json & j, domain::Route& x) {
        x.id = j.at("id").get<std::string>();
        x.brand_id = j.at("brand_id").get<std::string>();
        x.display_name = j.at("display_name").get<std::string>();
        x.affinities = j.at("affinities").get<std::vector<std::string>>();
        x.live_request_types = j.at("live_request_types").get<std::vector<std::string>>();
        x.color = j.at("color").get<std::string>();
        x.ui_color = j.at("ui_color").get<std::string>();
        x.text_color = j.at("text_color").get<std::string>();
    }

    inline void to_json(json & j, const domain::Route & x) {
        j = json::object();
        j["id"] = x.id;
        j["brand_id"] = x.brand_id;
        j["display_name"] = x.display_name;
        j["affinities"] = x.affinities;
        j["live_request_types"] = x.live_request_types;
        j["color"] = x.color;
        j["ui_color"] = x.ui_color;
        j["text_color"] = x.text_color;
    }

    inline void from_json(const json & j, domain::Vehicle& x) {
        x.headsign = j.at("headsign").get<std::string>();
        x.route_id = j.at("route_id").get<std::string>();
        x.pattern_id = j.at("pattern_id").get<std::string>();
        x.vehicle_id = j.at("vehicle_id").get<std::string>();
        x.stops_passed = j.at("stops_passed").get<double>();
        x.last_updated = j.at("last_updated").get<std::string>();
        x.coords = j.at("coords").get<std::vector<double>>();
    }

    inline void to_json(json & j, const domain::Vehicle & x) {
        j = json::object();
        j["headsign"] = x.headsign;
        j["route_id"] = x.route_id;
        j["pattern_id"] = x.pattern_id;
        j["vehicle_id"] = x.vehicle_id;
        j["stops_passed"] = x.stops_passed;
        j["last_updated"] = x.last_updated;
        j["coords"] = x.coords;
    }

    inline void from_json(const json & j, domain::VehicleLocation& x) {
        x.pattern_id = j.at("pattern_id").get<std::string>();
        x.vehicles = j.at("vehicles").get<std::vector<domain::Vehicle>>();
    }

    inline void to_json(json & j, const domain::VehicleLocation & x) {
        j = json::object();
        j["pattern_id"] = x.pattern_id;
        j["vehicles"] = x.vehicles;
    }

    inline void from_json(const json & j, domain::Welcome& x) {
        x.route = j.at("route").get<domain::Route>();
        x.vehicle_locations = j.at("vehicle_locations").get<std::vector<domain::VehicleLocation>>();
    }

    inline void to_json(json & j, const domain::Welcome & x) {
        j = json::object();
        j["route"] = x.route;
        j["vehicle_locations"] = x.vehicle_locations;
    }
}
