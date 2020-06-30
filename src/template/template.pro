TARGET = template

LIBS += -lws2_32

QT += \
    widgets
    gui

SOURCES += \
    Network/src/Address.cpp \
    Network/src/Broadcaster.cpp \
    Network/src/Client.cpp \
    Network/src/ClientSocket.cpp \
    Network/src/DataSocket.cpp \
    Network/src/DataStream.cpp \
    Network/src/Exception.cpp \
    Network/src/MeyaS.cpp \
    Network/src/Server.cpp \
    Network/src/ServerSocket.cpp \
    Network/src/Socket.cpp \
    Network/src/Timer.cpp \
    Network/src/Worker.cpp \
    Network/src/WorkerSocket.cpp \
    Top.cpp \
    main.cpp \
    ContentWidget.cpp \
    MainWindow.cpp \
    parse.cpp

HEADERS += \
    ContentWidget.h \
    MainWindow.h \
    Network/include/Address.h \
    Network/include/Broadcaster.h \
    Network/include/Client.h \
    Network/include/ClientSocket.h \
    Network/include/DataSocket.h \
    Network/include/DataStream.h \
    Network/include/Exception.h \
    Network/include/MeyaS.h \
    Network/include/Server.h \
    Network/include/ServerSocket.h \
    Network/include/Socket.h \
    Network/include/Timer.h \
    Network/include/Worker.h \
    Network/include/WorkerSocket.h \
    Network/include/common.h \
    parse.h

RESOURCES += \
  Resource.qrc

QMAKE_LFLAGS += -no-pie