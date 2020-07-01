#include "..\include\Timer.h"
#include "..\include\Server.h"

const std::vector<MeyaS::Worker *> &MeyaS::Server::getPeers() {
    return peers;
}

bool MeyaS::Server::startListening(MeyaS::uint peersDemanded) {
    uint cnt = 0;
    seats = peersDemanded;
    if(seats <= 0) return false;
    peers.clear();
    broadcaster = new Broadcaster();
    server = new ServerSocket(DEFAULT_PORT);
    server->bind();
    server->listen();
    return true;
}

MeyaS::Server::Server(): maxWaitTime(1), broadcaster(nullptr), server(nullptr), seats(0) {

}

bool MeyaS::Server::accept() {
    if(seats <= 0) return false;
    if(broadcaster== nullptr||server== nullptr) return false;
    DataPack data = DataPack((void *) "lo$", 4);
    broadcaster->shout(data);
    auto *ptr = server->accept();
    if (ptr != nullptr) {
        peers.emplace_back(new Worker(new DataStream(ptr)));
        seats--;
        return true;
    }
    return false;
}

bool MeyaS::Server::stopListening() {
    delete broadcaster; broadcaster = nullptr;
    delete server; server = nullptr;
    seats = 0;
    return true;
}

bool MeyaS::Server::isFullyConnected() const {
    return seats <= 0;
}
