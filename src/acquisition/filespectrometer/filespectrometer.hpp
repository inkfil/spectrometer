#pragma once

#include "../spectrometer.hpp"

#include <QString>

class FileSpectrometer : public ISpectrometer {
public:
    explicit FileSpectrometer(QString path, AcquisitionSettings fallbackSettings = {});
    ~FileSpectrometer() override;

    bool connect() override;
    void disconnect() override;
    bool isConnected() const override;
    AcquisitionResult acquire() override;

private:
    AcquisitionResult loadSpectrum() const;

    QString path;
    AcquisitionSettings fallbackSettings;
    Spectrum cachedSpectrum;
    bool connected = false;
};
