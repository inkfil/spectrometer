#include "logging.hpp"

#include <QDateTime>
#include <QDebug>
#include <QTextStream>

Q_LOGGING_CATEGORY(appLog, "spectrometer.app")
Q_LOGGING_CATEGORY(acquisitionLog, "spectrometer.acquisition")
Q_LOGGING_CATEGORY(processingLog, "spectrometer.processing")

namespace {
void spectrometerMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message) {
    const char* level = "INFO";

    switch (type) {
    case QtDebugMsg:
        level = "DEBUG";
        break;
    case QtInfoMsg:
        level = "INFO";
        break;
    case QtWarningMsg:
        level = "WARN";
        break;
    case QtCriticalMsg:
        level = "ERROR";
        break;
    case QtFatalMsg:
        level = "FATAL";
        break;
    }

    QTextStream stream(stderr);
    stream << QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs)
           << " [" << level << "] "
           << context.category << " "
           << message << Qt::endl;
}
}

void installSpectrometerMessageHandler() {
    qInstallMessageHandler(spectrometerMessageHandler);
}
