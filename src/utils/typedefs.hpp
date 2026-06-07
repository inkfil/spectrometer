#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>

typedef std::vector<float> floats;
typedef std::vector<double> doubles;
typedef std::vector<std::string> strings;
typedef std::vector<uint8_t> uint8_ts;
typedef std::vector<uint16_t> uint16_ts;
typedef std::vector<uint32_t> uint32_ts;
typedef std::vector<uint64_t> uint64_ts;

struct ColorRGB {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

namespace ColorUtils {

    inline double clamp(double v, double min = 0.0, double max = 1.0) {
        return std::max(min, std::min(max, v));
    }

    inline uint8_t to8bit(double value, double gamma, double factor) {
        if (value <= 0.0){
            return 0;
        }

        double corrected = std::pow(value * factor, gamma);
        corrected = clamp(corrected);

        return static_cast<uint8_t>(corrected * 255.0);
    }
}

// reference: https://github.com/leswright1977/PySpectrometer2/blob/main/src/specFunctions.py#L32
class WavelengthColorMapper {
public:
    static ColorRGB toRGB(double nm) {
        double r = 0.0, g = 0.0, b = 0.0;
        double factor = 0.0;

        constexpr double gamma = 0.8;

        // Base RGB
        if (nm >= 380 && nm <= 439) {
            r = -(nm - 440.0) / (440.0 - 380.0);
            g = 0.0;
            b = 1.0;
        }
        else if (nm >= 440 && nm <= 489) {
            r = 0.0;
            g = (nm - 440.0) / (490.0 - 440.0);
            b = 1.0;
        }
        else if (nm >= 490 && nm <= 509) {
            r = 0.0;
            g = 1.0;
            b = -(nm - 510.0) / (510.0 - 490.0);
        }
        else if (nm >= 510 && nm <= 579) {
            r = (nm - 510.0) / (580.0 - 510.0);
            g = 1.0;
            b = 0.0;
        }
        else if (nm >= 580 && nm <= 644) {
            r = 1.0;
            g = -(nm - 645.0) / (645.0 - 580.0);
            b = 0.0;
        }
        else if (nm >= 645 && nm <= 780) {
            r = 1.0;
            g = 0.0;
            b = 0.0;
        }

        // Intensity falloff factor
        if (nm >= 380 && nm <= 419) {
            factor = 0.3 + 0.7 * (nm - 380.0) / (420.0 - 380.0);
        }
        else if (nm >= 420 && nm <= 700) {
            factor = 1.0;
        }
        else if (nm >= 701 && nm <= 780) {
            factor = 0.3 + 0.7 * (780.0 - nm) / (780.0 - 700.0);
        }

        ColorRGB color;
        color.r = ColorUtils::to8bit(r, gamma, factor);
        color.g = ColorUtils::to8bit(g, gamma, factor);
        color.b = ColorUtils::to8bit(b, gamma, factor);

        // fallback gray (out of visible range)
        if (color.r == 0 && color.g == 0 && color.b == 0) {
            color = {155, 155, 155};
        }

        return color;
    }
};
