#pragma once
#include "../core/spectrum.hpp"
#include <QString>

struct AcquisitionResult {
    Spectrum spectrum;
    QString error;

    bool ok() const {
        return error.isEmpty() && spectrum.isValid();
    }
};

// Interface for acquisition
class ISpectrometer {
public:
    virtual ~ISpectrometer() = default;
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    virtual AcquisitionResult acquire() = 0;
};
