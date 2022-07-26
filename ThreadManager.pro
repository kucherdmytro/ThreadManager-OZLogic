QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src/
INCLUDEPATH += src/gui
INCLUDEPATH += src/data
INCLUDEPATH += src/models
INCLUDEPATH += src/handlers

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SOURCES += \
    src/gui/mainwindow.cpp \
    src/handlers/processreader.cpp \
    src/handlers/processreadrerlnx.cpp \
    src/main.cpp \
    src/models/processmodel.cpp \
    src/models/threadsmodel.cpp

FORMS += \
    src/gui/mainwindow.ui

HEADERS += \
    src/data/procdesc.h \
    src/gui/mainwindow.h \
    src/handlers/processreader.h \
    src/handlers/processreadrerlnx.h \
    src/models/processmodel.h \
    src/models/threadsmodel.h
