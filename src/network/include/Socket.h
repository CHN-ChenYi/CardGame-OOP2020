//
// Created by klxjt on 2020/5/13.
//

#pragma once

#include "common.h"

namespace MeyaS {
    class Socket {
    protected:
        virtual ~Socket();

        void shutdown();

        Socket();

        explicit Socket(addrinfo *addrInfo);

        void initialize(addrinfo *info);

        bool setBlocking(bool blocking);

        SOCKET sockfd; // An id for the socket
        addrinfo *addrInfo;
    };
}

