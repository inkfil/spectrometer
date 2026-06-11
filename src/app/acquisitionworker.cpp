#include "acquisitionworker.hpp"

#include "../acquisition/filespectrometer/filespectrometer.hpp"
#include "../acquisition/mockspectrometer/mockspectrometer.hpp"
#include "../core/logging.hpp"
#include "../processing/movingaverage.hpp"

AcquisitionWorker::AcquisitionWorker(AppConfig config, QObject* parent)
    : QObject(parent),
      config(config) {
    if (config.acquisitionSource == AcquisitionSource::FileReplay) {
        device = std::make_unique<FileSpectrometer>(config.replayFilePath, config.acquisition);
    } else {
        device = std::make_unique<MockSpectrometer>(config.acquisition, config.synthetic);
    }

    configurePipeline();
}

AcquisitionWorker::~AcquisitionWorker() {
    stop();
}

void AcquisitionWorker::start() {
    if (running) {
        return;
    }

    if (!timer) {
        timer = new QTimer(this);
        timer->setTimerType(Qt::PreciseTimer);
        connect(timer, &QTimer::timeout, this, &AcquisitionWorker::acquireOnce);
    }

    if (!device->isConnected() && !device->connect()) {
        emit acquisitionError("Unable to connect to spectrometer");
        return;
    }

    running = true;
    emit runningChanged(true);

    timer->start(config.acquisitionIntervalMs);
    qCInfo(acquisitionLog) << "Acquisition started at interval" << config.acquisitionIntervalMs << "ms";
}

void AcquisitionWorker::stop() {
    if (timer) {
        timer->stop();
    }

    if (device && device->isConnected()) {
        device->disconnect();
    }

    if (running) {
        running = false;
        emit runningChanged(false);
        qCInfo(acquisitionLog) << "Acquisition stopped";
    }
}

void AcquisitionWorker::acquireOnce() {
    const AcquisitionResult result = device->acquire();

    if (!result.ok()) {
        emit acquisitionError(result.error.isEmpty() ? "Invalid spectrum acquired" : result.error);
        return;
    }

    const Spectrum processed = pipeline.run(result.spectrum);
    QVector<double> wavelengths;
    QVector<double> intensities;
    wavelengths.reserve(processed.size());
    intensities.reserve(processed.size());

    for (qsizetype i = 0; i < processed.size(); ++i) {
        wavelengths.append(processed.wavelengths[static_cast<size_t>(i)]);
        intensities.append(processed.intensities[static_cast<size_t>(i)]);
    }

    emit spectrumReady(wavelengths, intensities);
}

void AcquisitionWorker::configurePipeline() {
    if (config.enableMovingAverage) {
        pipeline.add(std::make_unique<MovingAverage>());
    }
}
