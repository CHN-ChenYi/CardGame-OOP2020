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
        bool startListening(MeyaS::uint peersDemanded);

        //Check for connection
        bool accept();

        //Stop waiting for connection
        bool stopListening();

        //Check for space
        bool isFullyConnected() const;

        //Get all the connections
        const std::vector<Worker*>& getPeers();
    private:
        Broadcaster* broadcaster;
        ServerSocket* server;
        int seats;
        int maxWaitTime;
        std::vector<Worker*> peers;
    };
}
