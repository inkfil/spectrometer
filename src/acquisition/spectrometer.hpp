#pragma once
#include "../core/spectrum.hpp"

// Interface for acquisition
class ISpectrometer {
public:
    virtual ~ISpectrometer() = default;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual Spectrum acquire() = 0;
};
