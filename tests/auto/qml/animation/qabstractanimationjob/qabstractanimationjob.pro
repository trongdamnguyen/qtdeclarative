CONFIG += testcase parallel_test
macx:CONFIG -= app_bundle
TARGET = tst_qabstractanimationjob
QT = core-private qml-private testlib
SOURCES = tst_qabstractanimationjob.cpp
