#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "camera.h"
#include "identifiedobject.h"
#include "processor.h"
#include "logger.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("logger", &Logger::instance());

    qmlRegisterUncreatableType<Logger>("CalibrationSoft.Logger", 1, 0, "Logger", "Not creatable as it is an enum type.");

    Camera camera;
    IdentifiedObject paperSheet;
    Processor processor{&camera, &paperSheet};
    engine.rootContext()->setContextProperty("camera", &camera);
    engine.rootContext()->setContextProperty("paperSheet", &paperSheet);
    engine.rootContext()->setContextProperty("processor", &processor);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
