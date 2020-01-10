
#include <QDebug>
#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QString>
#include <dlt/dlt.h>
#include <dlt/dlt_types.h>

//원하는만큼 컨텍스트를 만들 수 있습니다. C 또는 CPP 파일에서 하나 이상의 컨텍스트를 선언 할 수 있습니다. 각 컨텍스트는 한 번만 선언 할 수 있습니다. 컨텍스트에 고유 한 변수 이름을 제공해야합니다.
DLT_DECLARE_CONTEXT(ctx); /* declare context */

// 다른 C 또는 CPP 파일에서 컨텍스트를 사용하려면 다음을 호출하여 컨텍스트를 가져올 수 있습니다.
// DLT_IMPORT_CONTEXT(ctx);

Q_LOGGING_CATEGORY(DevDigitalCluster, "DCLU.DigitalClusterHMI.Common");

int main()
{
    //응용 프로그램을 초기화하는 동안 가능한 한 빨리 응용 프로그램을 등록해야합니다. DLT_REGISTER_APP ()를 호출해야합니다. 애플리케이션에서 DLT_REGISTER_APP ()를 한 번만 호출 할 수 있습니다. 최대 4 개의 문자 길이 인 응용 프로그램 ID를 제공해야합니다. 이 예에서는 "MAPP"를 사용합니다. 또한 응용 프로그램에 대한 설명도 제공 할 수 있습니다. 여기에는 "로그 용 응용 프로그램 테스트"가 있습니다.
    DLT_REGISTER_APP("TAPP", "Test Application for Logging");

    //응용 프로그램을 등록한 후에는 응용 프로그램을 초기화하는 초기에 컨텍스트를 등록해야합니다. DLT_REGISTER_APP () 전에 DLT_REGISTER_CONTEXT ()를 호출하지 마십시오. 각 컨텍스트를 등록하는 동안 최대 4 자 길이의 컨텍스트 ID를 제공해야합니다. 이 예에서는 "TESX"를 사용합니다. 컨텍스트에 대한 설명도 제공 할 수 있습니다. 여기에는 "로깅을위한 테스트 컨텍스트 X"가 있습니다.
    DLT_REGISTER_CONTEXT(ctx, "TES1", "Test Context for Logging");

    /*
     * 로그 레벨은 다음 값 중 하나 여야합니다.
    DLT_LOG_FATAL	치명적인 시스템 오류
    DLT_LOG_ERROR	올바른 기능에 영향을 미치는 오류
    DLT_LOG_WARN	경고, 올바른 동작을 보장 할 수 없습니다
    DLT_LOG_INFO	정보 용 (기본값)
    DLT_LOG_DEBUG	디버그
    DLT_LOG_VERBOSE	최고급 정보

     * */

    DLT_LOG(ctx, DLT_LOG_ERROR, DLT_CSTRING("This is an error"));

    /* … */

    //응용 프로그램을 종료하기 전에 등록 된 모든 컨텍스트를 등록 취소하고 마지막으로 응용 프로그램을 등록 취소해야합니다.
    DLT_UNREGISTER_CONTEXT(ctx);
    DLT_UNREGISTER_APP();

    return 0;
}
