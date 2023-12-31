QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./mediator
INCLUDEPATH += ./net

LIBS += -lWs2_32

SOURCES += \
    chatdialog.cpp \
    ckernel.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    mediator/TcpClientMediator.cpp \
    mediator/TcpServerMediator.cpp \
    mediator/UdpMediator.cpp \
    net/INet.cpp \
    net/TcpClientNet.cpp \
    net/TcpServerNet.cpp \
    net/UdpNet.cpp \
    useritem.cpp

HEADERS += \
    chatdialog.h \
    ckernel.h \
    logindialog.h \
    mainwindow.h \
    mediator/INetMediator.h \
    mediator/TcpClientMediator.h \
    mediator/TcpServerMediator.h \
    mediator/UdpMediator.h \
    net/INet.h \
    net/TcpClientNet.h \
    net/TcpServerNet.h \
    net/UdpNet.h \
    net/packDef.h \
    useritem.h

FORMS += \
    chatdialog.ui \
    logindialog.ui \
    mainwindow.ui \
    useritem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    tx/0.png \
    tx/1.png \
    tx/10.png \
    tx/11.png \
    tx/12.png \
    tx/13.png \
    tx/14.png \
    tx/15.png \
    tx/16.png \
    tx/17.png \
    tx/18.png \
    tx/19.png \
    tx/2.png \
    tx/20.png \
    tx/21.png \
    tx/22.png \
    tx/23.png \
    tx/24.png \
    tx/25.png \
    tx/26.png \
    tx/27.png \
    tx/28.png \
    tx/29.png \
    tx/3.png \
    tx/30.png \
    tx/31.png \
    tx/32.png \
    tx/33.png \
    tx/34.png \
    tx/35.png \
    tx/4.png \
    tx/5.png \
    tx/6.png \
    tx/7.png \
    tx/8.png \
    tx/9.png
