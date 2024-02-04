QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controlarea.cpp \
    linear.cpp \
    main.cpp \
    mainwindow.cpp \
    mapcontrol.cpp \
    mapitem.cpp \
    maplist.cpp \
    mapproperties.cpp \
    mapshow.cpp \
    markerlist.cpp \
    markerproperties.cpp \
    scene.cpp \
    viewportitem.cpp

HEADERS += \
    controlarea.h \
    linear.h \
    mainwindow.h \
    mapcontrol.h \
    mapitem.h \
    maplist.h \
    mapproperties.h \
    mapshow.h \
    markerlist.h \
    markerproperties.h \
    scene.h \
    viewportitem.h

TRANSLATIONS += \
    SimpleRPGMap_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
