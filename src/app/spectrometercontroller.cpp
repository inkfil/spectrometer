#include "spectrometercontroller.hpp"
#include "../core/appconfig.hpp"
#include "../core/logging.hpp"

SpectrometerController::SpectrometerController(QObject* parent): QObject(parent) {
    qCInfo(appLog) << "Creating spectrometer controller";

    worker = new AcquisitionWorker(AppConfig::defaults());
    worker->moveToThread(&acquisitionThread);

    connect(&acquisitionThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SpectrometerController::startRequested, worker, &AcquisitionWorker::start);
    connect(this, &SpectrometerController::stopRequested, worker, &AcquisitionWorker::stop);

    connect(worker, &AcquisitionWorker::spectrumReady,
            this, &SpectrometerController::spectrumUpdated,
            Qt::QueuedConnection);

    connect(worker, &AcquisitionWorker::acquisitionError,
            this, [this](const QString& message) {
                qCWarning(acquisitionLog) << message;
                emit errorChanged(message);
            },
            Qt::QueuedConnection);

    acquisitionThread.start();
}

SpectrometerController::~SpectrometerController() {
    if (acquisitionThread.isRunning() && worker) {
        QMetaObject::invokeMethod(worker, "stop", Qt::BlockingQueuedConnection);
    }

    acquisitionThread.quit();
    acquisitionThread.wait();
    qCInfo(appLog) << "Destroyed spectrometer controller";
}

void SpectrometerController::start() {
    emit startRequested();
}

void SpectrometerController::stop() {
    emit stopRequested();
}
