#pragma once
#include "Socket.h"
#include "WorkerSocket.h"
#include "Address.h"
#define DEFAULT_PORT "12448"
namespace MeyaS {
    class WorkerSocket;
    class ServerSocket : public Socket {
    public:
        ServerSocket() = delete;

        explicit ServerSocket(const std::string &port);

        bool bind();

        bool listen();

        WorkerSocket* accept();
    private:
        std::string port;
    };
}