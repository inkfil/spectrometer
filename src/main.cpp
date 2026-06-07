#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>
#include <QQuickWindow>

using namespace Qt::StringLiterals;

#include "app/spectrometercontroller.hpp"
#include "app/spectrumview.hpp"

int main(int argc, char *argv[]) {

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    QApplication app(argc, argv);

    qmlRegisterType<SpectrumView>("Spectrometer", 1, 0, "SpectrumView");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/src/app/qml/main.qml"_s);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            qDebug() << "Obj: " << obj << ", ObjUrl: " << objUrl;
            if (!obj && url == objUrl){
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");

    SpectrometerController controller;
    engine.rootContext()->setContextProperty("controller", &controller);

    engine.load(url);
    qDebug() << "engine_object sizes: " << engine.rootObjects().size() << ", enfine.rootobj at 0: " << engine.rootObjects().at(0);

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
