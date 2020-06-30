
#include <Client.h>

#include "MeyaS.h"

int main() {
    //Initialize socket
    MeyaS::initialize();

    //Create client
    auto client = MeyaS::Client();

    //Connect to server
    auto serverList = client.probeServer();
    while (serverList.empty()) serverList = client.probeServer();
    client.connectTo(MeyaS::Address::createAddress(serverList.at(0), DEFAULT_PORT));

    //Send data
    auto stream = client.getPeer();
    std::cout << serverList.at(0) << std::endl;
    auto p = MeyaS::DataPack("Hello world$");
    stream->send(p);

    //Receive data
    std::string s = client.handleMessage();
    while (s.empty()) s = client.handleMessage();
    std::cout << s << std::endl;
}

