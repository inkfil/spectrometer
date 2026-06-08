#pragma once
#include "signalprocessor.hpp"

class MovingAverage : public ISignalProcessor {
public:
    MovingAverage();
    ~MovingAverage();

    Spectrum process(const Spectrum& input) override;
};
