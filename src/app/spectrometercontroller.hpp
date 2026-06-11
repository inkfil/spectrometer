#pragma once
#include <QObject>
#include <QVector>
#include <QThread>

#include "acquisitionworker.hpp"
#include "../core/appconfig.hpp"

class SpectrometerController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
    Q_PROPERTY(int acquisitionSource READ acquisitionSource NOTIFY configChanged)
    Q_PROPERTY(int syntheticProfile READ syntheticProfile NOTIFY configChanged)
    Q_PROPERTY(int spectrumDomain READ spectrumDomain NOTIFY configChanged)
    Q_PROPERTY(bool deterministic READ deterministic NOTIFY configChanged)
    Q_PROPERTY(int seed READ seed NOTIFY configChanged)
    Q_PROPERTY(QString replayFilePath READ replayFilePath NOTIFY configChanged)

public:
    SpectrometerController(QObject* parent = nullptr);
    ~SpectrometerController();

    bool isRunning() const;
    int acquisitionSource() const;
    int syntheticProfile() const;
    int spectrumDomain() const;
    bool deterministic() const;
    int seed() const;
    QString replayFilePath() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setAcquisitionSource(int source);
    Q_INVOKABLE void setSyntheticProfile(int profile);
    Q_INVOKABLE void setSpectrumDomain(int domain);
    Q_INVOKABLE void setDeterministic(bool deterministic);
    Q_INVOKABLE void setSeed(int seed);
    Q_INVOKABLE void setReplayFilePath(QString path);

signals:
    void spectrumUpdated(QVector<double> wavelengths,
                         QVector<double> intensities);
    void errorChanged(QString message);
    void runningChanged(bool running);
    void configChanged();
    void startRequested();
    void stopRequested();

private:
    void setupWorker();
    void teardownWorker();
    void applyConfigChange();

    AppConfig config = AppConfig::defaults();
    QThread acquisitionThread;
    AcquisitionWorker* worker = nullptr;
    bool running = false;
};
