QT += quick network concurrent
CONFIG += c++11
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
        Helpers/localization.cpp \
        Networking/httpproxyserver.cpp \
        ViewModels/configurationviewmodel.cpp \
        main.cpp \
        proxyemybackend.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Helpers/localization.h \
    Networking/httpproxyserver.h \
    ViewModels/configurationviewmodel.h \
    proxyemybackend.h
