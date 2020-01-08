#ifndef DLTREGISTRATION_H
#define DLTREGISTRATION_H

#include "dltregistration_global.h"

#include <dlt/dlt.h>

#include <QMap>
#include <QObject>
#include <QString>

class DltRegistrationPrivate;

class DLTREGISTRATIONSHARED_EXPORT DltRegistration : public QObject
{
    Q_OBJECT
public:
    DltRegistration();
    ~DltRegistration();

    void registerApplication(const char* dltAppID, const char* dltAppDescription);
    void unregisterApplication();

    void registerCategory(const QLoggingCategory* category, const char* dltCtxName, const char* dltCtxDescription);

    void setMinLogLevel(QtMsgType logLevel);

    DltContext* getDltContext(const QLoggingCategory* category);

    static void messageHandler(QtMsgType msgTypes, const QMessageLogContext& msgCtx, const QString& msg);

Q_SIGNALS:
    void logLevelChanged(const QLoggingCategory* category);

private:
    DltRegistrationPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(DltRegistration)

    friend void qtGeniviLogLevelChangedHandler(char context_id[], uint8_t log_level, uint8_t trace_status);
};

DLTREGISTRATIONSHARED_EXPORT extern DltRegistration* globalDltRegistration();

#endif // DLTREGISTRATION_H
