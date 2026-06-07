#pragma once
#include <QObject>
#include <QVector>
#include <QTimer>
#include <QDebug>

#include "../acquisition/spectrometer.hpp"
#include "../processing/processingpipeline.hpp"

class SpectrometerController : public QObject {
    Q_OBJECT

public:
    SpectrometerController(QObject* parent = nullptr);
    ~SpectrometerController();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:
    void spectrumUpdated(QVector<double> wavelengths,
                         QVector<double> intensities);

private:
    std::unique_ptr<ISpectrometer> device;
    ProcessingPipeline pipeline;
    QTimer timer;
};
