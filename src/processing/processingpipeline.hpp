#pragma once
#include "signalprocessor.hpp"
#include <memory>
#include <QLoggingCategory>
#include <vector>

Q_DECLARE_LOGGING_CATEGORY(processingLog)

class ProcessingPipeline {
public:
    void add(std::unique_ptr<ISignalProcessor> p) {
        processors.push_back(std::move(p));
    }

    Spectrum run(const Spectrum& input) {
        if (!input.isValid()) {
            qCWarning(processingLog) << "Skipping invalid spectrum in processing pipeline";
            return input;
        }

        Spectrum current = input;
        current.metadata.domain = SpectrumDomain::Processed;

        for (auto& p : processors) {
            current = p->process(current);
            if (!current.isValid()) {
                qCWarning(processingLog) << "Processor returned invalid spectrum";
                return input;
            }
        }

        return current;
    }

private:
    std::vector<std::unique_ptr<ISignalProcessor>> processors;
};
