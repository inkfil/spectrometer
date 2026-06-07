#include "movingaverage.hpp"

MovingAverage::MovingAverage(){

    qDebug() << "[MovingAverage::MovingAverage] this: " << this;
}

MovingAverage::~MovingAverage(){

    qDebug() << "[MovingAverage::~MovingAverage] this: " << this;
}

Spectrum MovingAverage::process(const Spectrum &input) {
    Spectrum out = input;

    for (size_t i = 1; i < input.intensities.size() - 1; ++i) {
        out.intensities[i] = (input.intensities[i - 1] + input.intensities[i] + input.intensities[i + 1]) / 3.0;
    }

    return out;
}
