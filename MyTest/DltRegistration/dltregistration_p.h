#ifndef DLTREGISTRATION_P
#define DLTREGISTRATION_P

#include <QHash>
#include <QString>

#include <dlt/dlt.h>

void qtGeniviLogLevelChangedHandler(char context_id[], uint8_t log_level, uint8_t trace_status);

class DltRegistration;

class DltRegistrationPrivate
{
public:
    DltRegistrationPrivate(DltRegistration* parent);

    void registerCategory(const QLoggingCategory* category, DltContext* dltContext, const char* dltCtxName, const char* dltCtxDescription);
    void setDefaultContext(DltContext* dltContext);

    void setMinLogLevel(QtMsgType logLevel);

    DltContext* context(const char* categoryName);
    void dltLogLevelChanged(char context_id[], uint8_t log_level, uint8_t trace_status);

private:
    DltRegistration* const q_ptr;
    Q_DECLARE_PUBLIC(DltRegistration)
    QString m_dltAppID;
    DltContext* m_defaultContext;
    QHash<QString, DltContext*> m_categoryName2DltContext;
    QHash<QString, QLoggingCategory*> m_ctxName2Category;
    QtMsgType m_minLevel = QtMsgType::QtDebugMsg;
};

#endif // DLTREGISTRATION_P
