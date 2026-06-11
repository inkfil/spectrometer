#include "spectrometercontroller.hpp"
#include "../core/logging.hpp"

SpectrometerController::SpectrometerController(QObject* parent): QObject(parent) {
    qCInfo(appLog) << "Creating spectrometer controller";
    setupWorker();
}

SpectrometerController::~SpectrometerController() {
    teardownWorker();
    qCInfo(appLog) << "Destroyed spectrometer controller";
}

bool SpectrometerController::isRunning() const {
    return running;
}

int SpectrometerController::acquisitionSource() const {
    return config.acquisitionSource == AcquisitionSource::FileReplay ? 1 : 0;
}

int SpectrometerController::syntheticProfile() const {
    return static_cast<int>(config.synthetic.profile);
}

int SpectrometerController::spectrumDomain() const {
    switch (config.synthetic.domain) {
    case SpectrumDomain::Dark:
        return 1;
    case SpectrumDomain::Reference:
        return 2;
    case SpectrumDomain::Raw:
    case SpectrumDomain::Processed:
        return 0;
    }

    return 0;
}

bool SpectrometerController::deterministic() const {
    return config.synthetic.deterministic;
}

int SpectrometerController::seed() const {
    return static_cast<int>(config.synthetic.seed);
}

QString SpectrometerController::replayFilePath() const {
    return config.replayFilePath;
}

void SpectrometerController::start() {
    emit startRequested();
}

void SpectrometerController::stop() {
    emit stopRequested();
}

void SpectrometerController::setAcquisitionSource(int source) {
    config.acquisitionSource = source == 1 ? AcquisitionSource::FileReplay : AcquisitionSource::MockSynthetic;
    applyConfigChange();
}

void SpectrometerController::setSyntheticProfile(int profile) {
    switch (profile) {
    case 1:
        config.synthetic.profile = SyntheticSampleProfile::AbsorbanceDye;
        break;
    case 2:
        config.synthetic.profile = SyntheticSampleProfile::PolymerBlend;
        break;
    case 3:
        config.synthetic.profile = SyntheticSampleProfile::DarkFrame;
        config.synthetic.domain = SpectrumDomain::Dark;
        break;
    case 4:
        config.synthetic.profile = SyntheticSampleProfile::ReferenceLamp;
        config.synthetic.domain = SpectrumDomain::Reference;
        break;
    case 0:
    default:
        config.synthetic.profile = SyntheticSampleProfile::EmissionStandard;
        break;
    }

    applyConfigChange();
}

void SpectrometerController::setSpectrumDomain(int domain) {
    switch (domain) {
    case 1:
        config.synthetic.domain = SpectrumDomain::Dark;
        break;
    case 2:
        config.synthetic.domain = SpectrumDomain::Reference;
        break;
    case 0:
    default:
        config.synthetic.domain = SpectrumDomain::Raw;
        break;
    }

    applyConfigChange();
}

void SpectrometerController::setDeterministic(bool deterministic) {
    config.synthetic.deterministic = deterministic;
    applyConfigChange();
}

void SpectrometerController::setSeed(int seed) {
    config.synthetic.seed = static_cast<quint32>(seed);
    applyConfigChange();
}

void SpectrometerController::setReplayFilePath(QString path) {
    config.replayFilePath = path.trimmed();
    applyConfigChange();
}

void SpectrometerController::setupWorker() {
    worker = new AcquisitionWorker(config);
    worker->moveToThread(&acquisitionThread);

    connect(&acquisitionThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SpectrometerController::startRequested, worker, &AcquisitionWorker::start);
    connect(this, &SpectrometerController::stopRequested, worker, &AcquisitionWorker::stop);

    connect(worker, &AcquisitionWorker::spectrumReady,
            this, &SpectrometerController::spectrumUpdated,
            Qt::QueuedConnection);

    connect(worker, &AcquisitionWorker::runningChanged,
            this, [this](bool isRunning) {
                if (running == isRunning) {
                    return;
                }

                running = isRunning;
                emit runningChanged(running);
            },
            Qt::QueuedConnection);

    connect(worker, &AcquisitionWorker::acquisitionError,
            this, [this](const QString& message) {
                qCWarning(acquisitionLog) << message;
                emit errorChanged(message);
            },
            Qt::QueuedConnection);

    acquisitionThread.start();
}

void SpectrometerController::teardownWorker() {
    if (acquisitionThread.isRunning() && worker) {
        QMetaObject::invokeMethod(worker, "stop", Qt::BlockingQueuedConnection);
    }

    acquisitionThread.quit();
    acquisitionThread.wait();
    worker = nullptr;

    if (running) {
        running = false;
        emit runningChanged(false);
    }
}

void SpectrometerController::applyConfigChange() {
    const bool restart = running;

    teardownWorker();
    setupWorker();

    emit configChanged();

    if (restart) {
        start();
    }
}
