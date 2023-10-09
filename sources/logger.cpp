#include "logger.h"

static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);

Logger::Logger(QObject *parent) : QObject(parent)
{
    qInstallMessageHandler(messageHandler);
}

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg.toLocal8Bit().constData());

    emit Logger::instance().logMessage(msg, static_cast<Logger::Type>(type));
}

QDebug Logger::success()
{
    return QDebug(QtDebugMsg).noquote();
}

QDebug Logger::info()
{
    return QDebug(QtInfoMsg).noquote();
}

QDebug Logger::warning()
{
    return QDebug(QtWarningMsg).noquote();
}

QDebug Logger::error()
{
    return QDebug(QtCriticalMsg).noquote();
}
