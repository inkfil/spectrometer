#include "mockspectrometer.hpp"
#include "../../core/logging.hpp"
#include <cmath>
#include <chrono>

MockSpectrometer::MockSpectrometer(AcquisitionSettings settings)
    : settings(settings) {
}

MockSpectrometer::~MockSpectrometer() = default;

bool MockSpectrometer::connect() {
    m_connected = true;
    qCInfo(acquisitionLog) << "Mock spectrometer connected";
    return true;
}

void MockSpectrometer::disconnect() {
    m_connected = false;
    qCInfo(acquisitionLog) << "Mock spectrometer disconnected";
}

bool MockSpectrometer::isConnected() const {
    return m_connected;
}

AcquisitionResult MockSpectrometer::acquire() {
    if (!m_connected) {
        return {{}, "Cannot acquire spectrum: spectrometer is not connected"};
    }

    if (settings.sampleCount < 2 || settings.endWavelengthNm <= settings.startWavelengthNm) {
        return {{}, "Invalid acquisition settings"};
    }

    Spectrum s;

    const int N = settings.sampleCount;
    const double start = settings.startWavelengthNm;
    const double end = settings.endWavelengthNm;
    const double exposureScale = std::max(0.05, settings.integrationTimeMs / 50.0);
    const double gain = std::max(0.0, settings.gain);

    std::normal_distribution<double> noise(0.0, 0.02);

    for (int i = 0; i < N; ++i) {
        double wl = start + (end - start) * i / (N - 1);

        // Simulated peaks
        double peak1 = std::exp(-0.5 * std::pow((wl - 500) / 10, 2));
        double peak2 = std::exp(-0.5 * std::pow((wl - 600) / 20, 2));
        double baseline = 0.08 + 0.0002 * (wl - start);

        double intensity = (baseline + peak1 + 0.7 * peak2) * exposureScale * gain + noise(rng);

        s.wavelengths.push_back(wl);
        s.intensities.push_back(intensity);
    }

    s.timestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    s.metadata.acquisition = settings;
    s.metadata.domain = SpectrumDomain::Raw;

    return {s, {}};
}
