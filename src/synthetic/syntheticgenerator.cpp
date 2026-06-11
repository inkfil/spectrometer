#include "syntheticgenerator.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <string>

namespace {
double gaussian(double wavelengthNm, const SyntheticPeak& peak) {
    return peak.amplitude * std::exp(-0.5 * std::pow((wavelengthNm - peak.centerNm) / peak.widthNm, 2));
}

double detectorRipple(double wavelengthNm, quint64 sequenceIndex) {
    const double phase = static_cast<double>(sequenceIndex % 360) * 0.017453292519943295;
    return 0.012 * std::sin(wavelengthNm * 0.035 + phase);
}

quint32 seedForConfig(const SyntheticSpectrumConfig& config) {
    if (!config.deterministic) {
        return std::random_device{}();
    }

    return config.seed + static_cast<quint32>(config.sequenceIndex * 7919);
}
}

Spectrum SyntheticSpectrumGenerator::generate(const AcquisitionSettings& settings,
                                              const SyntheticSpectrumConfig& config) {
    Spectrum spectrum;

    if (settings.sampleCount < 2 || settings.endWavelengthNm <= settings.startWavelengthNm) {
        return spectrum;
    }

    const int count = settings.sampleCount;
    const double start = settings.startWavelengthNm;
    const double end = settings.endWavelengthNm;
    const double exposureScale = std::max(0.05, settings.integrationTimeMs / 50.0);
    const double gain = std::max(0.0, settings.gain);
    const auto peaks = syntheticPeaksForProfile(config.profile);

    std::mt19937 rng(seedForConfig(config));
    std::normal_distribution<double> noise(0.0, config.noiseStdDev);

    spectrum.wavelengths.reserve(static_cast<size_t>(count));
    spectrum.intensities.reserve(static_cast<size_t>(count));

    for (int i = 0; i < count; ++i) {
        const double wavelength = start + (end - start) * i / (count - 1);
        double intensity = config.baselineOffset + config.baselineSlope * (wavelength - start);

        for (const auto& peak : peaks) {
            intensity += gaussian(wavelength, peak);
        }

        if (config.profile == SyntheticSampleProfile::DarkFrame || config.domain == SpectrumDomain::Dark) {
            intensity = config.darkCurrent + 0.004 * std::sin(wavelength * 0.09);
        } else if (config.profile == SyntheticSampleProfile::ReferenceLamp || config.domain == SpectrumDomain::Reference) {
            intensity += 0.18 + 0.2 * std::exp(-0.5 * std::pow((wavelength - 580.0) / 95.0, 2));
        }

        intensity += detectorRipple(wavelength, config.sequenceIndex);
        intensity = intensity * exposureScale * gain + noise(rng);
        intensity = std::clamp(intensity, 0.0, config.saturationLevel);

        spectrum.wavelengths.push_back(wavelength);
        spectrum.intensities.push_back(intensity);
    }

    spectrum.timestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    spectrum.metadata.acquisition = settings;
    spectrum.metadata.domain = config.domain;
    spectrum.metadata.sampleId = syntheticProfileName(config.profile);
    spectrum.metadata.tags["synthetic_profile"] = syntheticProfileName(config.profile).toStdString();
    spectrum.metadata.tags["synthetic_sequence"] = std::to_string(config.sequenceIndex);
    spectrum.metadata.tags["deterministic"] = config.deterministic ? "true" : "false";

    return spectrum;
}
