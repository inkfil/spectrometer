#pragma once
#include <QQuickItem>
#include <QVariantList>
#include <vector>
#include <mutex>

class SpectrumView : public QQuickItem {
    Q_OBJECT

public:
    SpectrumView();

    Q_INVOKABLE void setSpectrum(QVariantList wl, QVariantList intensity);

protected:
    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override;

private:
    std::vector<double> wavelengths;
    std::vector<double> intensities;

    std::vector<double> renderWavelengths;
    std::vector<double> renderIntensities;

    std::mutex dataMutex;
};
