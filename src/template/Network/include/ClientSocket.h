#pragma once

#include "DataSocket.h"
#include "Address.h"

namespace MeyaS {
    class ClientSocket : public DataSocket {
    public:
        explicit ClientSocket(Address *serverAddress);
        bool connect();
    private:
        Address* serverAddress;
    };
}