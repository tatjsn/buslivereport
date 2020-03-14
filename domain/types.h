#pragma once

#include <string>
#include <vector>

namespace domain {
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
