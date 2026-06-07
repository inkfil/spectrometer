#include "mockspectrometer.hpp"
#include <cmath>
#include <chrono>

MockSpectrometer::MockSpectrometer(){
    qDebug() << "[MockSpectrometer::MockSpectrometer] this: " << this;
}

MockSpectrometer::~MockSpectrometer(){
    qDebug() << "[MockSpectrometer::~MockSpectrometer] this: " << this;
}

void MockSpectrometer::connect() {
    m_connected = true;
}

void MockSpectrometer::disconnect() {
    m_connected = false;
}

Spectrum MockSpectrometer::acquire() {
    Spectrum s;

    const int N = 300;
    const double start = 400.0;
    const double end = 700.0;

    std::normal_distribution<double> noise(0.0, 0.02);

    for (int i = 0; i < N; ++i) {
        double wl = start + (end - start) * i / N;

        // Simulated peaks
        double peak1 = std::exp(-0.5 * std::pow((wl - 500) / 10, 2));
        double peak2 = std::exp(-0.5 * std::pow((wl - 600) / 20, 2));

        double intensity = peak1 + 0.7 * peak2 + noise(rng);

        s.wavelengths.push_back(wl);
        s.intensities.push_back(intensity);
    }

    s.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    return s;
}
