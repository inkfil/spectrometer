#include "movingaverage.hpp"

MovingAverage::MovingAverage() = default;
MovingAverage::~MovingAverage() = default;

Spectrum MovingAverage::process(const Spectrum &input) {
    Spectrum out = input;

    if (input.intensities.size() < 3) {
        return out;
    }

    for (size_t i = 1; i < input.intensities.size() - 1; ++i) {
        out.intensities[i] = (input.intensities[i - 1] + input.intensities[i] + input.intensities[i + 1]) / 3.0;
    }

    return out;
}
