#include "..\include\Timer.h"
#include "..\include\DataStream.h"

bool MeyaS::DataStream::send(const DataPack &data) {
    return socket->send(data);
}

MeyaS::DataPack *MeyaS::DataStream::recv(MeyaS::uint maxLength) {
    MeyaS::Timer t;
    t.start(maxWaitTime);
    while (!t.timeUp()) {
        auto ret = socket->recv(maxLength);
        if (ret != nullptr) return ret;
    }
    return nullptr;
}

MeyaS::DataStream::DataStream(MeyaS::DataSocket *socket) : socket(socket), maxWaitTime(1), identifier(L"Meyas"),
                                                           cache(), cacheW() {}

MeyaS::DataStream::~DataStream() {
    delete socket;
    socket = nullptr;
}

bool MeyaS::DataStream::sendLine(std::string s, char delimiter) {
    s += delimiter;
    auto t = DataPack(s);
    return send(t);
}

std::string MeyaS::DataStream::getLine(char delimiter) {
    std::string s;
    auto pos = cache.find(delimiter);
    if (pos != std::string::npos) { // Should be a full line
        s = cache.substr(0, pos);
        cache = cache.substr(pos + 1);
        return s;
    }
    MeyaS::Timer t;
    t.start(maxWaitTime);
    while (!t.timeUp()) {
        auto ret = socket->recv(512);
        if (ret != nullptr) {
            auto retString = std::string(reinterpret_cast<const char *>(ret->data));
            cache += retString;
            MeyaS::uint totalLength = retString.length()+1;
            while(totalLength<ret->length){ // More data to be retrieved
                retString = std::string(reinterpret_cast<const char *>(ret->data)+totalLength);
                cache += retString;
                totalLength += retString.length()+1;
            }
            delete ret;
            pos = cache.find(delimiter);
            if (pos != std::wstring::npos) { // Should be a full line
                s = cache.substr(0, pos);
                cache = cache.substr(pos + 1);
                return s;
            }
        }
    }
    return "";
}

void MeyaS::DataStream::setWaitTime(int time) {
    this->maxWaitTime = time;
}

bool MeyaS::DataStream::sendLineW(std::wstring s, wchar_t delimiter) {
    s += delimiter;
    auto t = DataPack(s);
    return send(t);
}

std::wstring MeyaS::DataStream::getLineW(wchar_t delimiter) {
    std::wstring s;
    auto pos = cacheW.find(delimiter);
    if (pos != std::wstring::npos) { // Should be a full line
        s = cacheW.substr(0, pos);
        cacheW = cacheW.substr(pos + 1);
        return s;
    }
    MeyaS::Timer t;
    t.start(maxWaitTime);
    while (!t.timeUp()) {
        auto ret = socket->recv(512);
        if (ret != nullptr) {
            auto retString = std::wstring(reinterpret_cast<const wchar_t *>(ret->data));
            cacheW += retString;
            MeyaS::uint totalLength = retString.length()+1;
            while(totalLength<ret->length/2){ // More data to be retrieved
                retString = std::wstring(reinterpret_cast<const wchar_t *>(ret->data)+totalLength);
                cacheW += retString;
                totalLength += retString.length()+1;
            }
            delete ret;
            pos = cacheW.find(delimiter);
            if (pos != std::wstring::npos) { // Should be a full line
                s = cacheW.substr(0, pos);
                cacheW = cacheW.substr(pos + 1);
                return s;
            }
        }
    }
    return L"";
}
