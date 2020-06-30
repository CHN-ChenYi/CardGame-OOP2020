#include "..\include\Timer.h"
#include "..\include\Server.h"

const std::vector<MeyaS::Worker *> &MeyaS::Server::getPeers() {
    return peers;
}

MeyaS::uint MeyaS::Server::startListening(MeyaS::uint peersDemanded) {
    uint cnt = 0;
    auto broadcaster = Broadcaster();
    auto server = ServerSocket(DEFAULT_PORT);
    server.bind();
    server.listen();
    Timer t{};
    t.start(maxWaitTime*100);
    DataPack data = DataPack((void *) "lo$", 4);
    while (!t.timeUp() && cnt < peersDemanded) {
        broadcaster.shout(data);
        auto *ptr = server.accept();
        if (ptr != nullptr) {
            peers.emplace_back(new Worker(new DataStream(ptr)));
            cnt++;
        }
    }
    return cnt;
}

MeyaS::Server::Server():maxWaitTime(3000) {

}
