#include "spectrometercontroller.hpp"
#include "../acquisition/mockspectrometer/mockspectrometer.hpp"
#include "../processing/movingaverage.hpp"

SpectrometerController::SpectrometerController(QObject* parent): QObject(parent){

    qDebug() << "[SpectrometerController::SpectrometerController] this: " << this;

    device = std::make_unique<MockSpectrometer>();
    device->connect();

    pipeline.add(std::make_unique<MovingAverage>());

    connect(&timer, &QTimer::timeout, this, [this]() {
        auto raw = device->acquire();
        auto processed = pipeline.run(raw);

        QVector<double> wl, in;
        for (size_t i = 0; i < processed.wavelengths.size(); ++i) {
            wl.append(processed.wavelengths[i]);
            in.append(processed.intensities[i]);
        }

        emit spectrumUpdated(wl, in);
    });
}

SpectrometerController::~SpectrometerController(){
    qDebug() << "[SpectrometerController::SpectrometerController] this: " << this;
}

void SpectrometerController::start() {
    timer.start(50); // ~20 FPS
}

void SpectrometerController::stop() {
    timer.stop();
}
