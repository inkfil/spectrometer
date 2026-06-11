#include "syntheticprofile.hpp"

QString syntheticProfileName(SyntheticSampleProfile profile) {
    switch (profile) {
    case SyntheticSampleProfile::EmissionStandard:
        return "emission-standard";
    case SyntheticSampleProfile::AbsorbanceDye:
        return "absorbance-dye";
    case SyntheticSampleProfile::PolymerBlend:
        return "polymer-blend";
    case SyntheticSampleProfile::DarkFrame:
        return "dark-frame";
    case SyntheticSampleProfile::ReferenceLamp:
        return "reference-lamp";
    }

    return "unknown";
}

std::vector<SyntheticPeak> syntheticPeaksForProfile(SyntheticSampleProfile profile) {
    switch (profile) {
    case SyntheticSampleProfile::EmissionStandard:
        return {
            {436.0, 0.55, 3.5},
            {486.1, 0.85, 4.0},
            {546.1, 1.25, 5.5},
            {656.3, 0.95, 6.0},
        };
    case SyntheticSampleProfile::AbsorbanceDye:
        return {
            {515.0, -0.38, 18.0},
            {585.0, -0.22, 28.0},
            {670.0, 0.12, 24.0},
        };
    case SyntheticSampleProfile::PolymerBlend:
        return {
            {430.0, 0.18, 18.0},
            {525.0, 0.42, 32.0},
            {610.0, 0.58, 26.0},
            {690.0, 0.22, 16.0},
        };
    case SyntheticSampleProfile::DarkFrame:
        return {};
    case SyntheticSampleProfile::ReferenceLamp:
        return {
            {410.0, 0.18, 24.0},
            {505.0, 0.32, 42.0},
            {590.0, 0.45, 52.0},
            {690.0, 0.22, 36.0},
        };
    }

    return {};
}
