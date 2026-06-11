#include "mockspectrometer.hpp"
#include "../../core/logging.hpp"
#include "../../synthetic/syntheticgenerator.hpp"

MockSpectrometer::MockSpectrometer(AcquisitionSettings settings,
                                   SyntheticSpectrumConfig syntheticConfig)
    : settings(settings),
      syntheticConfig(syntheticConfig) {
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

    syntheticConfig.sequenceIndex = sequenceIndex++;
    Spectrum s = SyntheticSpectrumGenerator::generate(settings, syntheticConfig);

    return {s, {}};
}
