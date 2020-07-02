#include "..\include\Exception.h"
#include "..\include\Socket.h"

MeyaS::Socket::Socket() {
    this->addrInfo = nullptr;
    this->sockfd = INVALID_SOCKET;
}

MeyaS::Socket::Socket(addrinfo *addrInfo) : sockfd(INVALID_SOCKET), addrInfo(nullptr) {
    initialize(addrInfo);
}

void MeyaS::Socket::initialize(addrinfo *info) {
    this->addrInfo = info;
    sockfd = socket(this->addrInfo->ai_family, this->addrInfo->ai_socktype, this->addrInfo->ai_protocol);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Initializing socket failed: " << WSAGetLastError() << std::endl;
        DebugException("Initializing socket failed");
    }
}

void MeyaS::Socket::shutdown() {
    closesocket(sockfd);
    sockfd = INVALID_SOCKET;
    freeaddrinfo(addrInfo);
    addrInfo = nullptr;
}

MeyaS::Socket::~Socket() {
    shutdown();
}

bool MeyaS::Socket::setBlocking(bool blocking) {
    if (sockfd < 0) return false;

#ifdef _WIN32
    unsigned long mode = blocking ? 0 : 1;
    return ioctlsocket(this->sockfd, FIONBIO, &mode) == 0;
#else
    int flags = fcntl(fd, F_GETFL, 0);
   if (flags == -1) return false;
   flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
   return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}
