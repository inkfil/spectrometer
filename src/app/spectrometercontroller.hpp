#pragma once
#include <QObject>
#include <QVector>
#include <QThread>

#include "acquisitionworker.hpp"

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
    void errorChanged(QString message);
    void startRequested();
    void stopRequested();

private:
    QThread acquisitionThread;
    AcquisitionWorker* worker = nullptr;
};
