
#include <Client.h>

#include "MeyaS.h"
int main(){
    MeyaS::initialize();
    auto client = MeyaS::Client();
    auto serverList = MeyaS::Client::probeServer();
    while(serverList.empty()) serverList = MeyaS::Client::probeServer();
    client.connectTo(MeyaS::Address::createAddress(serverList.at(0),DEFAULT_PORT));
    auto stream = client.getPeer();
    auto p = MeyaS::DataPack();
    p.data = reinterpret_cast<const byte *>("password$");
    p.length = 10;
    stream->send(p);
}

