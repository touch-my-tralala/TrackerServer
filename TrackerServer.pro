QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    logger/logger.cpp \
    main.cpp \
    mainwindow.cpp \
    resources/resourcemodel.cpp \
    resources/tablemodel.cpp \
    server/modelcontroller.cpp \
    server/servercontroller.cpp

HEADERS += \
    interfaces/observer.h \
    keys.h \
    logger/logger.h \
    mainwindow.h \
    resources/resourcemodel.h \
    resources/tablemodel.h \
    server/modelcontroller.h \
    server/servercontroller.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
