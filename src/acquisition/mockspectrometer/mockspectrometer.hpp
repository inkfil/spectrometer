#pragma once
#include "../spectrometer.hpp"
#include <random>
#include <QDebug>

class MockSpectrometer : public ISpectrometer {
public:
    MockSpectrometer();
    ~MockSpectrometer();

    void connect() override;
    void disconnect() override;
    Spectrum acquire() override;

private:
    bool m_connected = false;
    std::mt19937 rng{std::random_device{}()};
};
