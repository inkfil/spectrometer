#pragma once

#include "spectrum.hpp"
#include "../synthetic/syntheticprofile.hpp"

#include <QString>

enum class AcquisitionSource {
    MockSynthetic,
    FileReplay
};

struct AppConfig {
    AcquisitionSettings acquisition;
    AcquisitionSource acquisitionSource = AcquisitionSource::MockSynthetic;
    SyntheticSpectrumConfig synthetic;
    QString replayFilePath;
    int acquisitionIntervalMs = 50;
    bool autoConnect = true;
    bool enableMovingAverage = true;

    static AppConfig defaults() {
        AppConfig config;
        config.synthetic.profile = SyntheticSampleProfile::EmissionStandard;
        config.synthetic.domain = SpectrumDomain::Raw;
        config.synthetic.deterministic = true;
        return config;
    }
};
