QT -= gui

TEMPLATE = lib

DEFINES += DLTREGISTRATION_LIBRARY
DEFINES += __DEBUG__

DESTDIR = $$_PRO_FILE_PWD_/deploy

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR     = $$DESTDIR/.moc
RCC_DIR     = $$DESTDIR/.qrc
UI_DIR      = $$DESTDIR/.ui

target.path = $$_PRO_FILE_PWD_/../deploy
INSTALLS += target

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += -std=c++11 \
                                     -DDLT_2_18
QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter

#INVITE_LIBS = automotive-dlt

#CONFIG += link_pkgconfig

#PKGCONFIG += $$INVITE_LIBS

LIBS+= \
            -L/opt/GENIVI/lib -ldlt -lrt -lpthread


INCLUDEPATH+= /opt/GENIVI/include/dlt \
                            /opt/GENIVI/include \

SOURCES += \
        dltregistration.cpp

HEADERS += \
        dltregistration.h \
        dltregistration_global.h \
        dltregistration_p.h
