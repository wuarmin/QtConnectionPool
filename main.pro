TEMPLATE = subdirs

SUBDIRS = \
    QtConnectionPool \
    Tests

Tests.depends = QtConnectionPool
