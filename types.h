//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Welcome data = nlohmann::json::parse(jsonString);

#pragma once

#include "json.hpp"

namespace quicktype {
    using nlohmann::json;

    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }

    struct Route {
        std::string id;
        std::string brand_id;
        std::string display_name;
        std::vector<std::string> affinities;
        std::vector<std::string> live_request_types;
        std::string color;
        std::string ui_color;
        std::string text_color;
    };

    struct Vehicle {
        std::string headsign;
        std::string route_id;
        std::string pattern_id;
        std::string vehicle_id;
        double stops_passed;
        std::string last_updated;
        std::vector<double> coords;
    };

    struct VehicleLocation {
        std::string pattern_id;
        std::vector<Vehicle> vehicles;
    };

    struct Welcome {
        Route route;
        std::vector<VehicleLocation> vehicle_locations;
    };
}

namespace nlohmann {
    void from_json(const json & j, quicktype::Route & x);
    void to_json(json & j, const quicktype::Route & x);

    void from_json(const json & j, quicktype::Vehicle & x);
    void to_json(json & j, const quicktype::Vehicle & x);

    void from_json(const json & j, quicktype::VehicleLocation & x);
    void to_json(json & j, const quicktype::VehicleLocation & x);

    void from_json(const json & j, quicktype::Welcome & x);
    void to_json(json & j, const quicktype::Welcome & x);

    inline void from_json(const json & j, quicktype::Route& x) {
        x.id = j.at("id").get<std::string>();
        x.brand_id = j.at("brand_id").get<std::string>();
        x.display_name = j.at("display_name").get<std::string>();
        x.affinities = j.at("affinities").get<std::vector<std::string>>();
        x.live_request_types = j.at("live_request_types").get<std::vector<std::string>>();
        x.color = j.at("color").get<std::string>();
        x.ui_color = j.at("ui_color").get<std::string>();
        x.text_color = j.at("text_color").get<std::string>();
    }

    inline void to_json(json & j, const quicktype::Route & x) {
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

    inline void from_json(const json & j, quicktype::Vehicle& x) {
        x.headsign = j.at("headsign").get<std::string>();
        x.route_id = j.at("route_id").get<std::string>();
        x.pattern_id = j.at("pattern_id").get<std::string>();
        x.vehicle_id = j.at("vehicle_id").get<std::string>();
        x.stops_passed = j.at("stops_passed").get<double>();
        x.last_updated = j.at("last_updated").get<std::string>();
        x.coords = j.at("coords").get<std::vector<double>>();
    }

    inline void to_json(json & j, const quicktype::Vehicle & x) {
        j = json::object();
        j["headsign"] = x.headsign;
        j["route_id"] = x.route_id;
        j["pattern_id"] = x.pattern_id;
        j["vehicle_id"] = x.vehicle_id;
        j["stops_passed"] = x.stops_passed;
        j["last_updated"] = x.last_updated;
        j["coords"] = x.coords;
    }

    inline void from_json(const json & j, quicktype::VehicleLocation& x) {
        x.pattern_id = j.at("pattern_id").get<std::string>();
        x.vehicles = j.at("vehicles").get<std::vector<quicktype::Vehicle>>();
    }

    inline void to_json(json & j, const quicktype::VehicleLocation & x) {
        j = json::object();
        j["pattern_id"] = x.pattern_id;
        j["vehicles"] = x.vehicles;
    }

    inline void from_json(const json & j, quicktype::Welcome& x) {
        x.route = j.at("route").get<quicktype::Route>();
        x.vehicle_locations = j.at("vehicle_locations").get<std::vector<quicktype::VehicleLocation>>();
    }

    inline void to_json(json & j, const quicktype::Welcome & x) {
        j = json::object();
        j["route"] = x.route;
        j["vehicle_locations"] = x.vehicle_locations;
    }
}
