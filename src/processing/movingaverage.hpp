#pragma once
#include "signalprocessor.hpp"
#include <QDebug>

class MovingAverage : public ISignalProcessor {
public:
    MovingAverage();
    ~MovingAverage();

    Spectrum process(const Spectrum& input) override;
};
