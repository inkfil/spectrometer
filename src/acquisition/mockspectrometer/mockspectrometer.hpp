#pragma once
#include "../spectrometer.hpp"
#include "../../synthetic/syntheticprofile.hpp"

class MockSpectrometer : public ISpectrometer {
public:
    explicit MockSpectrometer(AcquisitionSettings settings = {},
                              SyntheticSpectrumConfig syntheticConfig = {});
    ~MockSpectrometer();

    bool connect() override;
    void disconnect() override;
    bool isConnected() const override;
    AcquisitionResult acquire() override;

private:
    AcquisitionSettings settings;
    SyntheticSpectrumConfig syntheticConfig;
    quint64 sequenceIndex = 0;
    bool m_connected = false;
};
