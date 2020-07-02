#include "..\include\Exception.h"
#include "..\include\Client.h"
#include "..\include\Broadcaster.h"

MeyaS::Broadcaster::Broadcaster() {
    addrinfo *result = nullptr, hints{};

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE;

    int iResult = getaddrinfo(nullptr, DEFAULT_SERVER_PROBE_PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "Getaddrinfo failed: " << iResult << std::endl;
        DebugException("Getaddrinfo failed");
    }
    initialize(result);
    const char *content = "1";
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, content, 1) < 0) {
        std::cerr << "Setting Broadcast option failed" << std::endl;
        closesocket(sockfd);
        sockfd = INVALID_SOCKET;
        DebugException("Setting Broadcast option failed");
    }
    type = 0;
}

bool MeyaS::Broadcaster::shout(const MeyaS::DataPack &dataPack) {
    if (type != 0 && type != BROADCAST_SEND) return false;
    type = BROADCAST_SEND;
    sockaddr_in sendAddr{};

    sendAddr.sin_family = AF_INET;
    sendAddr.sin_port = atoi(DEFAULT_SERVER_PROBE_PORT);
    sendAddr.sin_addr.s_addr = INADDR_BROADCAST;
    int iResult = sendto(sockfd, reinterpret_cast<const char *>(dataPack.data), dataPack.length, 0,
                         (sockaddr *) &sendAddr,
                         sizeof(sendAddr));
    if (iResult == SOCKET_ERROR) {
        auto err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) {
            std::cerr << "Sendto failed: " << err << std::endl;
            closesocket(sockfd);
            DebugException("Sendto failed");
        }
        return false;
    }
    return true;
}

bool MeyaS::Broadcaster::listen() {
    if (type != 0 && type != BROADCAST_RECV) return false;
    type = BROADCAST_RECV;
    setBlocking(false);
    sockaddr_in recvAddr{};
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = atoi(DEFAULT_SERVER_PROBE_PORT);
    recvAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (sockaddr *) &recvAddr, sizeof(recvAddr)) < 0) {
        std::cerr << "Binding broadcast listener failed: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        sockfd = INVALID_SOCKET;
        DebugException("Binding broadcast listener failed");
        return {};
    }
    return true;
}

std::pair<MeyaS::DataPack *, std::string> MeyaS::Broadcaster::accept() {
    if (type != BROADCAST_RECV) {
        DebugException("Broadcast accept failed");
        return {};
    }
    type = BROADCAST_RECV;
    sockaddr_in sender{};
    int len = sizeof(sockaddr_in);
    char recvbuff[50];
    int recvbufflen = 50;
    int iResult = recvfrom(sockfd, recvbuff, recvbufflen, 0, (sockaddr *) &sender, &len);
    if (iResult == SOCKET_ERROR) {
        auto err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) {
            std::cerr << "Accept failed: " << err << std::endl;
            closesocket(sockfd);
            DebugException("Server accept failed");
        }
        return {};
    }
    return std::make_pair(new DataPack(recvbuff, iResult), std::string(inet_ntoa(sender.sin_addr)));
}
