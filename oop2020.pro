TARGET = oop2020

LIBS += -lws2_32

QT += \
    widgets
    gui

SOURCES += \
    src/AI/AI.cpp \
    src/network/src/Address.cpp \
    src/network/src/Broadcaster.cpp \
    src/network/src/Client.cpp \
    src/network/src/ClientSocket.cpp \
    src/network/src/DataSocket.cpp \
    src/network/src/DataStream.cpp \
    src/network/src/Exception.cpp \
    src/network/src/MeyaS.cpp \
    src/network/src/Server.cpp \
    src/network/src/ServerSocket.cpp \
    src/network/src/Socket.cpp \
    src/network/src/Timer.cpp \
    src/network/src/Worker.cpp \
    src/network/src/WorkerSocket.cpp \
    src/template/ContentWidget.cpp \
    src/template/MainWindow.cpp \
    src/controller/Top.cpp \
    src/controller/main.cpp \
    src/controller/parse.cpp

HEADERS += \
    src/AI/AI.h \
    src/network/include/Address.h \
    src/network/include/Broadcaster.h \
    src/network/include/Client.h \
    src/network/include/ClientSocket.h \
    src/network/include/DataSocket.h \
    src/network/include/DataStream.h \
    src/network/include/Exception.h \
    src/network/include/MeyaS.h \
    src/network/include/Server.h \
    src/network/include/ServerSocket.h \
    src/network/include/Socket.h \
    src/network/include/Timer.h \
    src/network/include/Worker.h \
    src/network/include/WorkerSocket.h \
    src/network/include/common.h \
    src/template/ContentWidget.h \
    src/template/MainWindow.h \
    src/controller/Top.h \
    src/controller/parse.h

RESOURCES += \
    src/template/Resource.qrc

QMAKE_LFLAGS += -no-pie

DEFINES += QT_NO_DEBUG_OUTPUT

