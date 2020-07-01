#include "..\include\Timer.h"
#include "..\include\Exception.h"
#include "..\include\ClientSocket.h"

#define CONNECT_TIME_OUT 1000

MeyaS::ClientSocket::ClientSocket(Address *serverAddress) : serverAddress(serverAddress) {
    addrinfo *result = nullptr, hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    int iResult = getaddrinfo(this->serverAddress->getIP().c_str(), this->serverAddress->getPort().c_str(), &hints,
                              &result);
    if (iResult != 0) {
        std::cerr << "Getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        DebugException("Getaddrinfo failed");
    }
    ((sockaddr_in *) result->ai_addr)->sin_port = 12448;
    initialize(result);
    setBlocking(false);
}

bool MeyaS::ClientSocket::connect() {
    addrinfo *ptr = addrInfo;
    int iResult = SOCKET_ERROR;
    MeyaS::Timer t;
    while (ptr != nullptr && iResult == SOCKET_ERROR) {
        iResult = ::connect(sockfd, ptr->ai_addr, (int) ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            auto err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK) {
                std::cerr << err << std::endl;
            } else {
                timeval timeout{};
                timeout.tv_sec = CONNECT_TIME_OUT;
                timeout.tv_usec = 0;
                fd_set target;
                target.fd_count=1;
                target.fd_array[0] = sockfd;
                int res = ::select(0, nullptr, &target, nullptr, &timeout);
                if(res==0) continue; //Not writable
                else { // Writable, ok
                    iResult = 0;
                    break;
                }
            }
        }
        ptr = ptr->ai_next;
    }
    if (iResult == SOCKET_ERROR) {
        std::cerr << "Unable to connect to server!" << std::endl;
        closesocket(sockfd);
        freeaddrinfo(addrInfo);
        sockfd = INVALID_SOCKET;
        addrInfo = nullptr;
        WSACleanup();
        DebugException("Unable to connect to server");
        return false;
    }
    freeaddrinfo(addrInfo);
    addrInfo = nullptr;
    return true;
}
