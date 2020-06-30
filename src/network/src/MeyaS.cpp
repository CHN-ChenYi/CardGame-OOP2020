#include "..\include\MeyaS.h"

bool MeyaS::initialize(WORD version) {
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(version, &wsaData);
    if (iResult != 0) {
        std::cerr << "Winsock initialization failed: " << iResult <<std::endl;
        return false;
    }
    return true;
}
