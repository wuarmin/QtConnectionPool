TEMPLATE = lib
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
TARGET = qtconnectionpool

QT += sql

# Enable verbose behavior in debug mode
CONFIG(debug, debug|release) {
    DEFINES += VERBOSE
}

HEADERS += \
    $$PWD/database/poolconfig.h \
    $$PWD/database/databaseconfig.h \
    $$PWD/database/connectionpool.h \
    $$PWD/database/connectionpoolprivate.h \
    $$PWD/database/connection.h

SOURCES += \
    $$PWD/database/poolconfig.cpp \
    $$PWD/database/databaseconfig.cpp \
    $$PWD/database/connectionpool.cpp \
    $$PWD/database/connectionpoolprivate.cpp \
    $$PWD/database/connection.cpp

DISTFILES += \
    ../README.md

DESTDIR = ../bin