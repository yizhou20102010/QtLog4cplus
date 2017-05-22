QT += core
QT -= gui

CONFIG += c++11

TARGET = QtLog4cplusTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH+=$$quote(C:/Program Files (x86)/log4cplus/include)


win32:CONFIG(release, debug|release): LIBS += -L$$quote(C:/Program Files (x86)/log4cplus/libs/) -llog4cplus
else:win32:CONFIG(debug, debug|release): LIBS += -L$$quote(C:/Program Files (x86)/log4cplus/libs/) -llog4cplusD


INCLUDEPATH += $$quote(C:/Program Files (x86)/log4cplus/libs)
DEPENDPATH += $$quote(C:/Program Files (x86)/log4cplus/libs)

DISTFILES += \
    log4cplus.cfg
