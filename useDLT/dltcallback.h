#ifndef DLTCALLBACK_H
#define DLTCALLBACK_H

#include <QLoggingCategory>

class DLTCallback {
public:
    DLTCallback();
    static int testDLTCallback(uint32_t service_id, void* data, uint32_t length);
};

#endif // DLTCALLBACK_H
