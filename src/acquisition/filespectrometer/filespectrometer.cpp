#include "filespectrometer.hpp"

#include "../../core/logging.hpp"

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <chrono>
#include <utility>

FileSpectrometer::FileSpectrometer(QString path, AcquisitionSettings fallbackSettings)
    : path(std::move(path)),
      fallbackSettings(fallbackSettings) {
}

FileSpectrometer::~FileSpectrometer() = default;

bool FileSpectrometer::connect() {
    const AcquisitionResult result = loadSpectrum();
    if (!result.ok()) {
        qCWarning(acquisitionLog) << result.error;
        return false;
    }

    cachedSpectrum = result.spectrum;
    connected = true;
    qCInfo(acquisitionLog) << "Loaded replay spectrum from" << path;
    return true;
}

void FileSpectrometer::disconnect() {
    connected = false;
}

bool FileSpectrometer::isConnected() const {
    return connected;
}

AcquisitionResult FileSpectrometer::acquire() {
    if (!connected) {
        return {{}, "Cannot replay spectrum: file backend is not connected"};
    }

    Spectrum spectrum = cachedSpectrum;
    spectrum.timestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return {spectrum, {}};
}

AcquisitionResult FileSpectrometer::loadSpectrum() const {
    if (path.isEmpty()) {
        return {{}, "Replay file path is empty"};
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {{}, QString("Unable to open replay file: %1").arg(path)};
    }

    Spectrum spectrum;
    QTextStream stream(&file);

    while (!stream.atEnd()) {
        const QString line = stream.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) {
            continue;
        }

        const QString normalized = line;
        const QStringList parts = normalized.split(',', Qt::SkipEmptyParts);
        if (parts.size() < 2) {
            continue;
        }

        bool wavelengthOk = false;
        bool intensityOk = false;
        const double wavelength = parts.at(0).trimmed().toDouble(&wavelengthOk);
        const double intensity = parts.at(1).trimmed().toDouble(&intensityOk);

        if (!wavelengthOk || !intensityOk) {
            continue;
        }

        spectrum.wavelengths.push_back(wavelength);
        spectrum.intensities.push_back(intensity);
    }

    spectrum.metadata.acquisition = fallbackSettings;
    spectrum.metadata.acquisition.sampleCount = static_cast<int>(spectrum.wavelengths.size());
    spectrum.metadata.domain = SpectrumDomain::Raw;
    spectrum.metadata.sampleId = "file-replay";
    spectrum.metadata.deviceId = "file-spectrometer";
    spectrum.metadata.tags["source_file"] = path.toStdString();

    if (!spectrum.isValid()) {
        return {{}, QString("Replay file does not contain valid wavelength,intensity data: %1").arg(path)};
    }

    return {spectrum, {}};
}
