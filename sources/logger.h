#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QDebug>

/**
 * @brief The Logger class is a custom logger for this app.
 * It redirects console logs to this app ui with different levels
 */
class Logger : public QObject
{
    Q_OBJECT
public:
    enum Type {
            Info = QtInfoMsg,
            Warning = QtWarningMsg,
            Error = QtCriticalMsg,
            Success = QtDebugMsg // I'm using debug ass success to make it easier
    };
    Q_ENUM(Type)

    static Logger& instance();

    static QDebug success();
    static QDebug info();
    static QDebug warning();
    static QDebug error();

signals:
    void logMessage(const QString &message, Logger::Type level);

private:
    Logger(QObject *parent = nullptr);
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // LOGGER_H
