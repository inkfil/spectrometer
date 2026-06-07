#pragma once
#include "../core/spectrum.hpp"

// Processing interface
class ISignalProcessor {
public:
    virtual ~ISignalProcessor() = default;
    virtual Spectrum process(const Spectrum& input) = 0;
};
