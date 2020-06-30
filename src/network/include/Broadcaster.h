#pragma once
#define BROADCAST_RECV 1
#define BROADCAST_SEND 2

#include "Socket.h"
#include "Address.h"
#include "DataSocket.h"

namespace MeyaS {
    class Broadcaster : public Socket {
    public:
        Broadcaster();

        bool shout(const DataPack &dataPack);

        bool listen();

        std::pair<DataPack*, std::string> accept();

    private:
        uint type;
    };
}