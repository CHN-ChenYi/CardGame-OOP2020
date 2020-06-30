#pragma once

#include "common.h"
#include "Broadcaster.h"
#include "ServerSocket.h"
#include "Worker.h"

namespace MeyaS {
    class Server {
    public:
        Server();
        //Start broadcasting and waiting for connections. At most peersDemanded connections can be accepted.
        MeyaS::uint startListening(MeyaS::uint peersDemanded);
        //Get all the connections
        const std::vector<Worker*>& getPeers();
    private:
        int maxWaitTime;
        std::vector<Worker*> peers;
    };
}

