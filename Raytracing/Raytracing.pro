#-------------------------------------------------
#
# Project created by QtCreator 2019-05-25T14:52:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Raytracing
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

INCLUDEPATH += include

SOURCES += \
        src/main.cpp \
    src/primitives/box.cpp \
    src/primitives/quadrangle.cpp \
    src/primitives/sphere.cpp \
    src/about.cpp \
    src/canvas.cpp \
    src/figure.cpp \
    src/light.cpp \
    src/mainwindow.cpp \
    src/matrices.cpp \
    src/settings.cpp\
    src/raytracing.cpp \
    src/scenecanvas.cpp \
    src/primitives/triangle.cpp \
    src/parameters.cpp

HEADERS += \
    include/about.h \
    include/canvas.h \
    include/figure.h \
    include/light.h \
    include/mainwindow.h \
    include/matrices.h \
    include/settings.h \
    include/primitives/box.h \
    include/primitives/primitives.h \
    include/primitives/quadrangle.h \
    include/primitives/sphere.h \
    include/raytracing.h \
    include/primitives/triangle.h \
    include/scenecanvas.h \
    include/parameters.h

FORMS += \
        resources/forms/mainwindow.ui \
    resources/forms/about.ui\
    resources/forms/parameters.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/icons/icons.qrc \
    resources/images/images.qrc
