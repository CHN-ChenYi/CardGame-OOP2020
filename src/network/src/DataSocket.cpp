#include "..\include\Exception.h"
#include "..\include\DataSocket.h"

MeyaS::DataPack *MeyaS::DataSocket::recv(uint maxLength) {
    char *recvbuf = new char[maxLength + 2];
    recvbuf[maxLength] = 0;
    recvbuf[maxLength + 1] = 0;
    int iResult;
    iResult = ::recv(sockfd, recvbuf, maxLength, 0);
    if (iResult == SOCKET_ERROR) {
        auto err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK && err != WSAETIMEDOUT) {
            //Notice here, if client socket is blocking and has a recv timeout, it may returns WSAETIMEDOUT
            std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            DebugException("Recv failed");
        }
        delete[] recvbuf;
        return nullptr;
    }
    std::clog << iResult << " bytes of data received." << std::endl;
    auto *ret = new DataPack(recvbuf, iResult);
    return ret;
}

int MeyaS::DataSocket::send(const DataPack &dataPack) {
    int iResult = ::send(sockfd, reinterpret_cast<const char *>(dataPack.data), dataPack.length, 0);
    if (iResult == SOCKET_ERROR) {
        auto err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            DebugException("Send failed");
            return err;
        }
        return 0;
    }
    std::clog << iResult << " bytes of data sent." << std::endl;
    return 0;
}

//MeyaS::DataPack::DataPack() : data(nullptr), length(0), type(0) {}

MeyaS::DataPack::DataPack(void *data, MeyaS::uint length) : length(length), type(0) {
    this->data = new byte[length];
    memcpy((void *) this->data, data, length);
}

MeyaS::DataPack::~DataPack() {
    delete[] this->data;
    this->data = nullptr;
}

MeyaS::DataPack::DataPack(const std::string &data) {
    length = data.length() + 1;
    this->data = new byte[length];
    memcpy((void *) this->data, data.c_str(), length);
}

MeyaS::DataPack::DataPack(const std::wstring &data) {
    length = (data.length() + 1) * sizeof(data[0]);
    this->data = new byte[length];
    memcpy((void *) this->data, data.c_str(), length);
}
