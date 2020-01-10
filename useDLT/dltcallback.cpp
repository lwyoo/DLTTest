#include "dltcallback.h"
#include <QDebug>
#include <dlt/dlt.h>
DLT_IMPORT_CONTEXT(myContext1);

DLTCallback::DLTCallback()
{
}

int DLTCallback::testDLTCallback(uint32_t service_id, void* data, uint32_t length)
{
    Q_UNUSED(service_id);
    Q_UNUSED(data);
    Q_UNUSED(length);
    DLT_LOG(myContext1, DLT_LOG_ERROR, DLT_INT(5), DLT_STRING("This is a error1111111111111111111"));

    return 0;
}
