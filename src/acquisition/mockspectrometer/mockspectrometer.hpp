#pragma once
#include "../spectrometer.hpp"
#include <random>

class MockSpectrometer : public ISpectrometer {
public:
    explicit MockSpectrometer(AcquisitionSettings settings = {});
    ~MockSpectrometer();

    bool connect() override;
    void disconnect() override;
    bool isConnected() const override;
    AcquisitionResult acquire() override;

private:
    AcquisitionSettings settings;
    bool m_connected = false;
    std::mt19937 rng{std::random_device{}()};
};
