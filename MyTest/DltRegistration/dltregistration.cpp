// polyspace-begin MISRA-CPP:2-10-1 [Justified:Low] "[Maintainability] Applied rule exception - Different identifiers shall be typographically unambiguous" 
// polyspace-begin MISRA-CPP:2-10-2 [Justified:Low] "[Maintainability] Applied rule exception - Identifiers declared in an inner scope shall not hide an identifier declared in an outer scope." 
// polyspace-begin MISRA-CPP:5-2-1 [Justified:Low] "[Maintainability] Applied rule exception - Each operand of a logical  or || shall be a postfix expression." 
// polyspace-begin MISRA-CPP:6-6-5 [Justified:Low] "[Maintainability] Applied rule exception - A function shall have a single point of exit at the end of the function." 
// polyspace-begin MISRA-CPP:16-2-1 [Justified:Low] "[Maintainability] Applied rule exception - The pre-processor shall only be used for file inclusion and include guards." 
// polyspace-begin MISRA-CPP:18-4-1 [Justified:Low] "[Maintainability] Applied rule exception - Dynamic heap memory allocation shall not be used."

#include "dltregistration.h"
#include "dltregistration_p.h"

#include <QDebug>
#include <QLoggingCategory>

DLT_DECLARE_CONTEXT(ContextDefault)

void qtGeniviLogLevelChangedHandler(char context_id[], uint8_t log_level, uint8_t trace_status)
{
    globalDltRegistration()->d_ptr->dltLogLevelChanged(context_id, log_level, trace_status); // polyspace DEFECT:NULL_PTR [Justified:High] "[Reliability] pointer is created as global static variable. there is no exception case. There is no side-effect"
}

Q_GLOBAL_STATIC(DltRegistration, dltRegistration) // polyspace DEFECT:INLINE_CONSTRAINT_NOT_RESPECTED [Not a defect:Medium] "Qt에서 제공하는 MACRO 사용으로 인하여 자동 생성된 코드에서 검출된 항목으로 위험 요소 없음"

DltRegistration* globalDltRegistration()
{
    return dltRegistration();
}

DltRegistrationPrivate::DltRegistrationPrivate(DltRegistration* parent)
    : q_ptr(parent)
    , m_defaultContext(nullptr)
{
    DLT_REGISTER_APP("DCLU", "DCLU.DltRegistration.Default");
    DLT_REGISTER_CONTEXT(ContextDefault, "-", "DCLU.DltRegistration.Default");
    setDefaultContext(&ContextDefault);
}

void DltRegistrationPrivate::registerCategory(const QLoggingCategory* category, DltContext* dltContext, const char* dltCtxName, const char* dltCtxDescription)
{

    //    DLT_REGISTER_CONTEXT(*dltContext, dltCtxName, dltCtxDescription);
    DLT_REGISTER_CONTEXT_LL_TS(*dltContext, dltCtxName, dltCtxDescription, DLT_LOG_DEBUG, DLT_TRACE_STATUS_ON); //    DLT_TRACE_STATUS_DEFAULT DLT_LOG_DEFAULT
    m_categoryName2DltContext.insert(QString::fromLatin1(category->categoryName()), dltContext);
    m_ctxName2Category.insert(QString::fromLatin1(dltCtxName), const_cast<QLoggingCategory*>(category));

#ifdef DLT_VERSION_2_12
    //TODO move to lamda once c++11 is ok to be used
    DLT_REGISTER_LOG_LEVEL_CHANGED_CALLBACK(*dltContext, &qtGeniviLogLevelChangedHandler);
#endif
}

void DltRegistrationPrivate::setDefaultContext(DltContext* dltContext)
{
    m_defaultContext = dltContext;
}

void DltRegistrationPrivate::setMinLogLevel(QtMsgType logLevel)
{
#if QT_VERSION >= 0x050500
    QtMsgType convertType = QtWarningMsg;
    /*
     QtDebugMsg     ->  QtDebugMsg
     QtWarningMsg   ->  QtCriticalMsg
     QtCriticalMsg  ->  QtFatalMsg
     QtFatalMsg     ->  QtInfoMsg
     QtInfoMsg      ->  QtWarningMsg
    */
    switch (logLevel) {
    case QtDebugMsg:
        convertType = QtDebugMsg;
        break;
    case QtWarningMsg:
        convertType = QtCriticalMsg;
        break;
    case QtCriticalMsg:
        convertType = QtFatalMsg;
        break;
    case QtFatalMsg:
        convertType = QtInfoMsg;
        break;
    case QtInfoMsg:
        convertType = QtWarningMsg;
        break;
    }
    m_minLevel = convertType;
#else
    if (logLevel == QtInfoMsg) {
        logLevel = QtWarningMsg;
    }
    m_minLevel = logLevel;
#endif
}

DltContext* DltRegistrationPrivate::context(const char* categoryName)
{
    const QString category = QString::fromLatin1(categoryName);
    if (!m_categoryName2DltContext.contains(category) && m_defaultContext)
        return m_defaultContext;

    return m_categoryName2DltContext.value(category);
}

void DltRegistrationPrivate::dltLogLevelChanged(char context_id[], uint8_t log_level, uint8_t trace_status)
{
    Q_Q(DltRegistration);
    Q_UNUSED(trace_status)

    const QString contextName = QString::fromLatin1(context_id);

    if (m_ctxName2Category.contains(contextName)) {
        QList<QtMsgType> msgTypes;

        switch (log_level) {
        case DLT_LOG_VERBOSE:
        case DLT_LOG_DEBUG:
            msgTypes << QtDebugMsg << QtWarningMsg << QtCriticalMsg << QtFatalMsg;
#if QT_VERSION >= 0x050500
            msgTypes << QtInfoMsg;
#endif
            break;
        case DLT_LOG_INFO:
            msgTypes << QtWarningMsg << QtCriticalMsg << QtFatalMsg;
#if QT_VERSION >= 0x50500
            msgTypes << QtInfoMsg;
#endif
            break;
        case DLT_LOG_WARN:
            msgTypes << QtCriticalMsg << QtFatalMsg;
#if QT_VERSION >= 0x50500
            msgTypes << QtInfoMsg;
#endif
            break;
        case DLT_LOG_ERROR:
            msgTypes << QtCriticalMsg << QtFatalMsg;
            break;
        case DLT_LOG_FATAL:
            msgTypes << QtFatalMsg;
            break;
        case DLT_LOG_OFF:
            break;
        default:
            break;
        }

        QtMsgType end = QtFatalMsg;
        //#if QT_VERSION >= 0x50500
        //        end << QtInfoMsg;
        //#endif

        for (int i = (int)QtDebugMsg; i <= (int)end; i++) {
            QtMsgType type = (QtMsgType)i;
            bool enabled = true;

            if (!msgTypes.contains(type)) {
                enabled = false;
            }

            QLoggingCategory* category = m_ctxName2Category.value(contextName);

            if ((category != nullptr) && (category->isEnabled(type) != enabled)) {
                category->setEnabled(type, enabled);
            }
        }
    }
}

DltRegistration::DltRegistration()
    : d_ptr(new DltRegistrationPrivate(this))
{
}

DltRegistration::~DltRegistration()
{
    fflush(stderr);
    unregisterApplication();
    if (d_ptr != nullptr) {
        delete d_ptr;
    }
}

void DltRegistration::registerApplication(const char* dltAppID, const char* dltAppDescription)
{
    Q_D(DltRegistration);
    d->m_dltAppID = QString::fromLatin1(dltAppID);
    DLT_REGISTER_APP(dltAppID, dltAppDescription);
}

void DltRegistration::registerCategory(const QLoggingCategory* category, const char* dltCtxName, const char* dltCtxDescription)
{
    Q_D(DltRegistration);
    d->registerCategory(category, new DltContext, dltCtxName, dltCtxDescription);
}

void DltRegistration::setMinLogLevel(QtMsgType logLevel)
{
    Q_D(DltRegistration);
    d->setMinLogLevel(logLevel);
}

DltContext* DltRegistration::getDltContext(const QLoggingCategory* category)
{
    Q_D(DltRegistration);
    return d->context(category->categoryName());
}

void DltRegistration::unregisterApplication()
{
    Q_D(DltRegistration);

    if (!d->m_dltAppID.isEmpty())
        DLT_UNREGISTER_APP();
}

void DltRegistration::messageHandler(QtMsgType msgTypes, const QMessageLogContext& msgCtx, const QString& msg)
{
    /*
     QtDebugMsg     ->  QtDebugMsg
     QtWarningMsg   ->  QtCriticalMsg
     QtCriticalMsg  ->  QtFatalMsg
     QtFatalMsg     ->  QtInfoMsg
     QtInfoMsg      ->  QtWarningMsg
    */

    QtMsgType convertType = QtWarningMsg;
    switch (msgTypes) {
    case QtDebugMsg:
        convertType = QtDebugMsg;
        break;
    case QtWarningMsg:
        convertType = QtCriticalMsg;
        break;
    case QtCriticalMsg:
        convertType = QtFatalMsg;
        break;
    case QtFatalMsg:
        convertType = QtInfoMsg;
        break;
    case QtInfoMsg:
        convertType = QtWarningMsg;
        break;
    }

    if (static_cast<int32_t>(convertType) < static_cast<int32_t>(globalDltRegistration()->d_ptr->m_minLevel)) { // polyspace DEFECT:NULL_PTR [Justified:High] "[Reliability] pointer is created as global static variable. there is no exception case. There is no side-effect"
        // pass...
        return;
    }

#if ORIGINAL_QDLTREG
    if (msgTypes < globalDltRegistration()->d_ptr->m_minLevel) {
        // pass...
        return;
    }
#endif

#ifdef __RELEASE__
    switch ((int)msgTypes) {
    case QtDebugMsg:
#if QT_VERSION >= 0x050500
    case QtInfoMsg:
#endif
    case QtWarningMsg:
        fprintf(stderr, "[%s]%s %s\n", msgCtx.category, msgCtx.function, msg.toLatin1().data());
        break;
    case QtCriticalMsg:
    case QtFatalMsg:
        fprintf(stderr, "[%s]%s %s\n", msgCtx.category, msgCtx.function, msg.toLatin1().data());
        fflush(stderr);
        break;
    }

#elif __DEBUG__
    DltContext* dltCtx = globalDltRegistration()->d_ptr->context(msgCtx.category); // polyspace DEFECT:NULL_PTR [Justified:High] "[Reliability] pointer is created as global static variable. there is no exception case. There is no side-effect"
    if (!dltCtx)
        return;

    DltLogLevelType logLevel = DLT_LOG_DEBUG; //DLT_LOG_INFO;

    switch (msgTypes) {
    case QtDebugMsg:
        logLevel = DLT_LOG_DEBUG;
        break;
#if QT_VERSION >= 0x050500
    case QtInfoMsg:
        logLevel = DLT_LOG_INFO;
        break;
#endif
    case QtWarningMsg:
        logLevel = DLT_LOG_WARN;
        break;
    case QtCriticalMsg:
        logLevel = DLT_LOG_ERROR;
        break;
    case QtFatalMsg:
        logLevel = DLT_LOG_FATAL;
        break;
    }
#define VIA_DLT
#ifdef VIA_DLT
    DLT_LOG(*dltCtx, logLevel, DLT_STRING(qPrintable(qFormatLogMessage(msgTypes, msgCtx, msg))));
#endif

#ifdef VIA_SERIAL
    fprintf(stderr, "%s \n", qFormatLogMessage(msgTypes, msgCtx, msg).toStdString().c_str());
#endif

#endif
}

// polyspace-end MISRA-CPP:2-10-1 [Justified:Low] "[Maintainability] Applied rule exception - Different identifiers shall be typographically unambiguous" 
// polyspace-end MISRA-CPP:2-10-2 [Justified:Low] "[Maintainability] Applied rule exception - Identifiers declared in an inner scope shall not hide an identifier declared in an outer scope." 
// polyspace-end MISRA-CPP:5-2-1 [Justified:Low] "[Maintainability] Applied rule exception - Each operand of a logical  or || shall be a postfix expression." 
// polyspace-end MISRA-CPP:6-6-5 [Justified:Low] "[Maintainability] Applied rule exception - A function shall have a single point of exit at the end of the function." 
// polyspace-end MISRA-CPP:16-2-1 [Justified:Low] "[Maintainability] Applied rule exception - The pre-processor shall only be used for file inclusion and include guards." 
// polyspace-end MISRA-CPP:18-4-1 [Justified:Low] "[Maintainability] Applied rule exception - Dynamic heap memory allocation shall not be used."

