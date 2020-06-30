#pragma once

#include "ServerSocket.h"
#include "DataSocket.h"

namespace MeyaS {
    class ServerSocket;

    class WorkerSocket : public DataSocket {
        friend class ServerSocket;

    public:
        const std::string &getPeerIP();

    private:
        explicit WorkerSocket(SOCKET sockfd, std::string ip);

        std::string ip;
    };
}
