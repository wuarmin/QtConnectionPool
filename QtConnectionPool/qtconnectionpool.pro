INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += sql

# Enable verbose behavior in debug mode
CONFIG(debug, debug|release) {
    DEFINES += VERBOSE
}

HEADERS += \
    $$PWD/database/connectionpoolprivate.h \
    $$PWD/database/databaseconnection.h \
    $$PWD/database/poolconfig.h \
    $$PWD/database/databaseconfig.h \
    $$PWD/database/connectionpool.h \
    $$PWD/database/connectionprivate.h \
    $$PWD/database/connection.h

SOURCES += \
    $$PWD/database/connectionpoolprivate.cpp \
    $$PWD/database/databaseconnection.cpp \
    $$PWD/database/poolconfig.cpp \
    $$PWD/database/databaseconfig.cpp \
    $$PWD/database/connectionpool.cpp \
    $$PWD/database/connectionprivate.cpp \
    $$PWD/database/connection.cpp

DISTFILES += \
    ../README.md
