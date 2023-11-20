QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addnew.cpp \
    client.cpp \
    friendsmodel.cpp \
    groupchat.cpp \
    groupsmodel.cpp \
    listdelegate.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    membersdelegate.cpp \
    membersmodel.cpp \
    newgroup.cpp \
    privatechat.cpp \
    recvcommands.cpp \
    recvmanager.cpp \
    recvprocessor.cpp \
    registerUp.cpp \
    self.cpp \
    sendcommands.cpp \
    setting.cpp \
    socket.cpp

HEADERS += \
    addnew.h \
    client.h \
    friendsmodel.h \
    groupchat.h \
    groupsmodel.h \
    listdelegate.h \
    login.h \
    mainwindow.h \
    membersdelegate.h \
    membersmodel.h \
    newgroup.h \
    privatechat.h \
    public.h \
    recvcommands.h \
    recvmanager.h \
    recvprocessor.h \
    registerUp.h \
    self.h \
    sendcommands.h \
    setting.h \
    socket.h

FORMS += \
    addnew.ui \
    groupchat.ui \
    login.ui \
    mainwindow.ui \
    newgroup.ui \
    privatechat.ui \
    registerUp.ui \
    setting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
