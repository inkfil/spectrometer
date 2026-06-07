#pragma once
#include "signalprocessor.hpp"
#include <memory>
#include <vector>

class ProcessingPipeline {
public:

    ProcessingPipeline(){
        qDebug() << "[ProcessingPipeline::ProcessingPipeline] this: " << this;
    }

    ~ProcessingPipeline(){
        qDebug() << "[ProcessingPipeline::~ProcessingPipeline] this: " << this;
    }

    void add(std::unique_ptr<ISignalProcessor> p) {
        processors.push_back(std::move(p));
    }

    Spectrum run(const Spectrum& input) {
        Spectrum current = input;
        for (auto& p : processors)
            current = p->process(current);
        return current;
    }

private:
    std::vector<std::unique_ptr<ISignalProcessor>> processors;
};
