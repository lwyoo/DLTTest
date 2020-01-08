#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>

#include "../DltRegistration/dltregistration.h"

Q_LOGGING_CATEGORY(DevDigitalCluster, "DCLU.DigitalClusterHMI.Common");

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    DltRegistration* dlt = globalDltRegistration();

    dlt->registerApplication("hmia1", "HMDC.ClusterHMI1");

    dlt->registerCategory(&DevDigitalCluster(), "dccm1", "HMDC.ClusterHMI.Common1");

    globalDltRegistration()->setMinLogLevel(QtDebugMsg);

    qInstallMessageHandler(&(DltRegistration::messageHandler));

    qCDebug(DevDigitalCluster) << "dldyddn ";

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
