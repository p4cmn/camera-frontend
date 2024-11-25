QT = core network

CONFIG += c++21 cmdline

SOURCES += \
        Client/tcpclient.cpp \
        Controller/mediacontroller.cpp \
        main.cpp \
        view/inputthread.cpp \
        view/mediaview.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Client/tcpclient.h \
    Controller/mediacontroller.h \
    view/inputthread.h \
    view/mediaview.h

DISTFILES += \
    .gitignore
