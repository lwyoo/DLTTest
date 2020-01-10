#include <QDebug>
#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QString>
#include <dlt/dlt.h>
#include <dlt/dlt_types.h>

//원하는만큼 컨텍스트를 만들 수 있습니다. C 또는 CPP 파일에서 하나 이상의 컨텍스트를 선언 할 수 있습니다. 각 컨텍스트는 한 번만 선언 할 수 있습니다. 컨텍스트에 고유 한 변수 이름을 제공해야합니다.
DLT_DECLARE_CONTEXT(ctx); /* declare context */
Q_LOGGING_CATEGORY(DevDigitalCluster, "DCLU.DigitalClusterHMI.Common");

static void messageHandler(QtMsgType msgTypes, const QMessageLogContext& msgCtx, const QString& msg);

int main()
{
    //    DLT_REGISTER_APP("TAPP", "Test Application for Logging");

    //    DLT_REGISTER_CONTEXT(ctx, "TES1", "Test Context for Logging");

    //    /* … */

    //    DLT_LOG(ctx, DLT_LOG_ERROR, DLT_CSTRING("This is an error"));

    //    /* … */

    //    DLT_UNREGISTER_CONTEXT(ctx);
    //    DLT_UNREGISTER_APP();

    DLT_REGISTER_APP("hmi1", "hmi_description");
    DLT_REGISTER_CONTEXT(ctx, "TES1", "Test Context for Logging");
    qInstallMessageHandler(&(messageHandler));

    //    DLT_LOG(ctx, DLT_LOG_ERROR, DLT_CSTRING("This is an error dldyddn "));
    qCDebug(DevDigitalCluster) << "dldyddn ";
    qCInfo(DevDigitalCluster) << "dldyddn info";
    return 0;
}
void messageHandler(QtMsgType msgTypes, const QMessageLogContext& msgCtx, const QString& msg)
{
    DLT_LOG(ctx, DLT_LOG_INFO, DLT_STRING(qPrintable(qFormatLogMessage(msgTypes, msgCtx, msg))));
}
