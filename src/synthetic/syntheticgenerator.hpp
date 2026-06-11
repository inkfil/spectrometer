#pragma once

#include "syntheticprofile.hpp"

class SyntheticSpectrumGenerator {
public:
    static Spectrum generate(const AcquisitionSettings& settings,
                             const SyntheticSpectrumConfig& config);
};
