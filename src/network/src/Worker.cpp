#include "..\include\Timer.h"
#include "..\include\Worker.h"

MeyaS::Worker::Worker(DataStream *p) : maxWaitTime(3000), peer(p), alive(true) {
}

MeyaS::DataStream *MeyaS::Worker::getPeer() {
    return peer;
}

std::wstring MeyaS::Worker::checkStatus() {
    auto ret = peer->sendLineW(L"#heart");
    if (ret != 0) {
        alive = false;
        return L"";
    }
    MeyaS::Timer t;
    t.start(maxWaitTime);
    do {
        auto s = peer->getLineW();
        if (s.empty()) continue;
        return s;
    } while (!t.timeUp());
    alive = false;
    return L"";
}

void MeyaS::Worker::shutdown() {
    peer->sendLineW(L"#shutdown");
    alive = false;
}

MeyaS::Worker::~Worker() {
    delete peer;
}

bool MeyaS::Worker::isAlive() const {
    return alive;
}
