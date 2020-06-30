#pragma once
#include "DataStream.h"
#include "Address.h"
#define DEFAULT_SERVER_PROBE_PORT "12449"
namespace MeyaS {
    class Client {
    public:
        Client();
        ~Client();

        //Connect to server
        bool connectTo(Address* address);

        //Probe for broadcasting server ips
        std::vector<std::string> probeServer() const;

        //Handle incoming messages. Will handle pre-defined message like heartbeat and shutdown.
        //Return the incoming message. Return empty string is no full line of message(ended with delimiter)
        //is received.
        std::wstring handleMessage();

        //Get ad DataStream for sending and receiving data.
        DataStream* getPeer();

        //Check if the connection is alive
        bool isAlive() const;
    private:
        bool alive;
        int maxWaitTime;
        DataStream* peer;
    };
}