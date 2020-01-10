//https://at.projects.genivi.org/wiki/display/PROJ/DLT+cheatsheet
#include <dlt/dlt.h>
#include <dlt/dlt_types.h>

DLT_DECLARE_CONTEXT(myContext1);
DLT_DECLARE_CONTEXT(myContext2);
DLT_DECLARE_CONTEXT(myContext3);

int main()
{
    /* register application */
    DLT_REGISTER_APP("MAPP", "Test Application for Logging");

    /* register all contexts */
    DLT_REGISTER_CONTEXT(myContext1, "TES1", "Test Context 1 for Logging");
    DLT_REGISTER_CONTEXT(myContext2, "TES2", "Test Context 2 for Logging");
    DLT_REGISTER_CONTEXT(myContext3, "TES3", "Test Context 3 for Logging");

    /* Write your logs */
    DLT_LOG(myContext1, DLT_LOG_ERROR, DLT_INT(5), DLT_STRING("This is a error"));
    DLT_LOG(myContext2, DLT_LOG_INFO, DLT_INT(5), DLT_STRING("But this only information"));
    DLT_LOG(myContext3, DLT_LOG_DEBUG, DLT_INT(5), DLT_STRING("But this only information"));

    /* Sleep some time to avoid a flaw in dlt-daemon that would eat your messages
         if you deregister while it still processes your registration */
    sleep(3);

    /* unregister your contexts */
    DLT_UNREGISTER_CONTEXT(myContext1);
    DLT_UNREGISTER_CONTEXT(myContext2);
    DLT_UNREGISTER_CONTEXT(myContext3);

    /* unregister your application */
    DLT_UNREGISTER_APP();

    return 0;
}
