//https://at.projects.genivi.org/wiki/display/PROJ/DLT+cheatsheet
#include "dltcallback.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <dlt/dlt.h>
#include <dlt/dlt_types.h>

DLT_DECLARE_CONTEXT(myContext1);
DLT_DECLARE_CONTEXT(myContext2);

int main(int argc, char* argv[])
{
    /* register application */
    DLT_REGISTER_APP("MAPP", "Test Application for Logging");

    /* register all contexts */
    DLT_REGISTER_CONTEXT(myContext1, "TES1", "Test Context 1 for Logging");
    DLT_REGISTER_CONTEXT(myContext2, "TES2", "Test Context 2 for Logging");

    /* Write your logs */
    DLT_LOG(myContext1, DLT_LOG_ERROR, DLT_INT(5), DLT_STRING("This is a error"));
    DLT_LOG(myContext2, DLT_LOG_INFO, DLT_INT(5), DLT_STRING("But this only information"));

    //    DLT_REGISTER_INJECTION_CALLBACK()

    /* Sleep some time to avoid a flaw in dlt-daemon that would eat your messages
         if you deregister while it still processes your registration */
    sleep(3);

    DLT_REGISTER_INJECTION_CALLBACK(myContext1, 0xfff, DLTCallback::testDLTCallback);
    /* unregister your contexts */

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();

    //    DLT_UNREGISTER_CONTEXT(myContext1);
    //    DLT_UNREGISTER_CONTEXT(myContext2);

    //    /* unregister your application */
    //    DLT_UNREGISTER_APP();

    //    return 0;
}
