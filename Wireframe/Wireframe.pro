#-------------------------------------------------
#
# Project created by QtCreator 2019-05-16T14:50:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wireframe
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

CONFIG += c++11

SOURCES += \
        main.cpp \
    default.cpp \
    figure.cpp \
    canvas.cpp \
    bspline.cpp \
    mainwindow.cpp \
    parameters.cpp \
    parameters2.cpp \
    scenecanvas.cpp \
    settings.cpp \
    about.cpp \
    bsplinecanvas.cpp

HEADERS += \
    bspline.h \
    bsplinecanvas.h \
    canvas.h \
    figure.h \
    mainwindow.h \
    matrices.h \
    parameters2.h \
    parameters.h \
    settings.h \
    about.h \
    scenecanvas.h

FORMS += \
        resources/forms/mainwindow.ui \
        resources/forms/parameters.ui \
        resources/forms/parameters.ui \
        resources/forms/parameters2.ui \
    resources/forms/about.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/icons/icons.qrc \
    resources/images/images.qrc

