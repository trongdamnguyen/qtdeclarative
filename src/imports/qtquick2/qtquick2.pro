CXX_MODULE = qml
TARGET  = qtquick2plugin
TARGETPATH = QtQuick
IMPORT_VERSION = 2.15

SOURCES += \
    plugin.cpp

QT += quick-private qml-private qmlmodels-private

qtConfig(qml-worker-script): QT += qmlworkerscript-private

load(qml_plugin)
