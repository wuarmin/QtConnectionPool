INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += sql

# Enable verbose behavior in debug mode
CONFIG(debug, debug|release) {
    DEFINES += VERBOSE
}

HEADERS += \
    $$PWD/database/databaseconnectionpool.h \
    $$PWD/database/databaseconnection.h \
    $$PWD/database/poolconfig.h \
    $$PWD/database/databaseconfig.h

SOURCES += \
    $$PWD/database/databaseconnectionpool.cpp \
    $$PWD/database/databaseconnection.cpp \
    $$PWD/database/poolconfig.cpp \
    $$PWD/database/databaseconfig.cpp
