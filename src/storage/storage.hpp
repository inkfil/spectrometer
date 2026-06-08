#pragma once

#include "../core/spectrum.hpp"
#include <string>

// Storage interface
class IStorage {
public:
    virtual ~IStorage() = default;
    virtual void save(const Spectrum&, const std::string& path) = 0;
    virtual Spectrum load(const std::string& path) = 0;
};
