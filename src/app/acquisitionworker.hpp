#pragma once

#include "../acquisition/spectrometer.hpp"
#include "../core/appconfig.hpp"
#include "../processing/processingpipeline.hpp"

#include <QObject>
#include <QTimer>
#include <QVector>
#include <memory>

class AcquisitionWorker : public QObject {
    Q_OBJECT

public:
    explicit AcquisitionWorker(AppConfig config, QObject* parent = nullptr);
    ~AcquisitionWorker() override;

public slots:
    void start();
    void stop();

signals:
    void spectrumReady(QVector<double> wavelengths, QVector<double> intensities);
    void acquisitionError(QString message);
    void runningChanged(bool running);

private:
    void acquireOnce();
    void configurePipeline();

    AppConfig config;
    std::unique_ptr<ISpectrometer> device;
    ProcessingPipeline pipeline;
    QTimer* timer = nullptr;
    bool running = false;
};
