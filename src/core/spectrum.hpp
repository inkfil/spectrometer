#pragma once
#include "../utils/typedefs.hpp"
#include <QString>
#include <QtGlobal>
#include <map>
#include <string>

enum class SpectrumDomain {
    Raw,
    Processed,
    Dark,
    Reference
};

struct AcquisitionSettings {
    double integrationTimeMs = 50.0;
    double gain = 1.0;
    double startWavelengthNm = 400.0;
    double endWavelengthNm = 700.0;
    int sampleCount = 300;
};

struct SpectrumMetadata {
    QString sampleId;
    QString operatorId;
    QString deviceId = "mock-spectrometer";
    QString methodId = "default";
    QString wavelengthUnit = "nm";
    QString intensityUnit = "a.u.";
    SpectrumDomain domain = SpectrumDomain::Raw;
    AcquisitionSettings acquisition;
    std::map<std::string, std::string> tags;
};

struct Spectrum {
    std::vector<double> wavelengths;
    std::vector<double> intensities;
    quint64 timestampMs = 0;
    SpectrumMetadata metadata;

    bool isValid() const {
        return !wavelengths.empty()
            && wavelengths.size() == intensities.size()
            && metadata.acquisition.sampleCount > 0;
    }

    qsizetype size() const {
        return static_cast<qsizetype>(wavelengths.size());
    }
};
