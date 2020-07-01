#include "..\include\Timer.h"
#include "..\include\Worker.h"

MeyaS::Worker::Worker(DataStream *p) : maxWaitTime(3000), peer(p),alive(true) {
}

MeyaS::DataStream *MeyaS::Worker::getPeer() {
    return peer;
}

bool MeyaS::Worker::checkStatus() {
    peer->sendLineW(L"#heart");
    MeyaS::Timer t;
    t.start(maxWaitTime);
    while (!t.timeUp()) {
        auto s = peer->getLineW();
        if (s.empty()) continue;
        if(s == L"#beat"){
            return true;
        }else{
            alive = false;
            return false;
        }
    }
    alive = false;
    return false;
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
