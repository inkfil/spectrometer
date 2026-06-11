#pragma once

#include "../core/spectrum.hpp"

#include <QString>
#include <vector>

enum class SyntheticSampleProfile {
    EmissionStandard,
    AbsorbanceDye,
    PolymerBlend,
    DarkFrame,
    ReferenceLamp
};

struct SyntheticPeak {
    double centerNm = 0.0;
    double amplitude = 1.0;
    double widthNm = 10.0;
};

struct SyntheticSpectrumConfig {
    SyntheticSampleProfile profile = SyntheticSampleProfile::EmissionStandard;
    SpectrumDomain domain = SpectrumDomain::Raw;
    quint32 seed = 1337;
    quint64 sequenceIndex = 0;
    double noiseStdDev = 0.015;
    double baselineOffset = 0.08;
    double baselineSlope = 0.0002;
    double darkCurrent = 0.025;
    double saturationLevel = 4.0;
    bool deterministic = true;
};

QString syntheticProfileName(SyntheticSampleProfile profile);
std::vector<SyntheticPeak> syntheticPeaksForProfile(SyntheticSampleProfile profile);
