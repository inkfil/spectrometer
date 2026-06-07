#pragma once
#include "../utils/typedefs.hpp"
#include <QDebug>

// Spectrum data container
struct Spectrum {
    Spectrum(){
        qDebug() << "[Spectrum::Spectrum] this: " << this;
    }

    ~Spectrum(){
        qDebug() << "[Spectrum::~Spectrum] this: " << this;
    }

    std::vector<double> wavelengths;
    std::vector<double> intensities;
    uint64_t timestamp;
};
