#pragma once

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(appLog)
Q_DECLARE_LOGGING_CATEGORY(acquisitionLog)
Q_DECLARE_LOGGING_CATEGORY(processingLog)

void installSpectrometerMessageHandler();
