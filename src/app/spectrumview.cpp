#include "spectrumview.hpp"
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QSGVertexColorMaterial>
#include <QSGGeometry>
#include <QColor>

#include "../utils/typedefs.hpp"

SpectrumView::SpectrumView() {
    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);}

void SpectrumView::setSpectrum(QVariantList wlList,
                               QVariantList intList) {

    std::lock_guard<std::mutex> lock(dataMutex);

    wavelengths.clear();
    intensities.clear();

    for (const auto& v : wlList){
        wavelengths.push_back(v.toDouble());
    }

    for (const auto& v : intList){
        intensities.push_back(v.toDouble());
    }

    update();
}
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGVertexColorMaterial>
#include <algorithm>

QSGNode* SpectrumView::updatePaintNode(QSGNode* oldNode,
                                       UpdatePaintNodeData*)
{
    qDebug() << "updatePaintNode called";

    // -------------------------------
    // 1. Thread-safe copy
    // -------------------------------
    std::vector<double> wl, in;
    {
        std::lock_guard<std::mutex> lock(dataMutex);
        wl = wavelengths;
        in = intensities;
    }

    int N = wl.size();
    qDebug() << "points:" << N;

    // -------------------------------
    // 2. Early exit
    // -------------------------------
    if (N < 2) {
        qDebug() << "Not enough data";
        return oldNode;
    }

    // -------------------------------
    // 3. Create node if needed
    // -------------------------------
    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(oldNode);

    if (!node) {
        node = new QSGGeometryNode();

        auto* geometry = new QSGGeometry(
            QSGGeometry::defaultAttributes_ColoredPoint2D(),
            0);

        geometry->setDrawingMode(QSGGeometry::DrawTriangles);

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        // ✅ CORRECT MATERIAL (per-vertex color)
        auto* material = new QSGVertexColorMaterial();
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    QSGGeometry* geometry = node->geometry();

    // -------------------------------
    // 4. Allocate correct vertex count
    // -------------------------------
    int vertexCount = (N - 1) * 6;  // 2 triangles per segment
    geometry->allocate(vertexCount);

    qDebug() << "Allocating vertices:" << vertexCount;

    auto* vertices = geometry->vertexDataAsColoredPoint2D();

    float w = width();
    float h = height();

    // -------------------------------
    // 5. Normalize intensity
    // -------------------------------
    double maxVal = *std::max_element(in.begin(), in.end());
    if (maxVal == 0.0) maxVal = 1.0;

    // -------------------------------
    // 6. Fill geometry
    // -------------------------------
    for (int i = 0; i < N - 1; ++i) {

        float x1 = float(i) / (N - 1) * w;
        float x2 = float(i + 1) / (N - 1) * w;

        float y1 = h - float(in[i] / maxVal) * h;
        float y2 = h - float(in[i + 1] / maxVal) * h;

        auto c1 = WavelengthColorMapper::toRGB(wl[i]);
        auto c2 = WavelengthColorMapper::toRGB(wl[i + 1]);

        int idx = i * 6;

        // Triangle 1
        vertices[idx + 0].set(x1, h, c1.r, c1.g, c1.b, 255);
        vertices[idx + 1].set(x1, y1, c1.r, c1.g, c1.b, 255);
        vertices[idx + 2].set(x2, y2, c2.r, c2.g, c2.b, 255);

        // Triangle 2
        vertices[idx + 3].set(x1, h, c1.r, c1.g, c1.b, 255);
        vertices[idx + 4].set(x2, y2, c2.r, c2.g, c2.b, 255);
        vertices[idx + 5].set(x2, h, c2.r, c2.g, c2.b, 255);
    }

    // -------------------------------
    // 7. Notify scene graph
    // -------------------------------

    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}
