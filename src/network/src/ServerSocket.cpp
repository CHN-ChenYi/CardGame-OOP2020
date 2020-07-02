#include "..\include\Exception.h"
#include "..\include\Address.h"
#include "..\include\ServerSocket.h"
#include "..\include\WorkerSocket.h"

/*MeyaS::ServerSocket::ServerSocket() {
    addrinfo *result = nullptr, hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    int iResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        ();
        DebugException("Getaddrinfo failed");
    }
    initialize(result);
    setBlocking(false);
}*/

MeyaS::ServerSocket::ServerSocket(const std::string &port): port(port) {
    addrinfo *result = nullptr, hints{};

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int iResult = getaddrinfo(nullptr, port.c_str(), &hints, &result);
    if (iResult != 0) {
        std::cerr << "Getaddrinfo failed: " << iResult << std::endl;
        DebugException("Getaddrinfo failed");
    }
    initialize(result);
    setBlocking(false);
}

bool MeyaS::ServerSocket::bind() {
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = stoi(port);
    int iResult = ::bind(sockfd, reinterpret_cast<const sockaddr *>(&server), sizeof(server));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(addrInfo);
        closesocket(sockfd);
        addrInfo = nullptr;
        sockfd = INVALID_SOCKET;
        DebugException("Bind socket failed");
        return false;
    }
    freeaddrinfo(addrInfo);
    addrInfo = nullptr;
    return true;
}

bool MeyaS::ServerSocket::listen() {
    int iResult = ::listen(sockfd, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        sockfd = INVALID_SOCKET;
        DebugException("Bind socket failed");
        return false;
    }
    return true;
}

MeyaS::WorkerSocket *MeyaS::ServerSocket::accept() {
    auto workerSockfd = INVALID_SOCKET;
    sockaddr addr{};
    int addrlen = sizeof(sockaddr);
    workerSockfd = ::accept(sockfd, &addr, &addrlen);
    sockaddr_in *addrin = (reinterpret_cast<sockaddr_in *>(&addr));

    if (workerSockfd == INVALID_SOCKET) {
        auto err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) {
            std::cerr << "Accept failed: " << err << std::endl;
            closesocket(sockfd);
            DebugException("Server accept failed");
        }
        return nullptr;
    }
    return new WorkerSocket(workerSockfd, std::string(inet_ntoa(addrin->sin_addr)));
}
