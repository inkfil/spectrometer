#pragma once

#include "spectrum.hpp"

struct AppConfig {
    AcquisitionSettings acquisition;
    int acquisitionIntervalMs = 50;
    bool autoConnect = true;
    bool enableMovingAverage = true;

    static AppConfig defaults() {
        AppConfig config;
        return config;
    }
};
