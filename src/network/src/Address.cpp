#include <sstream>
#include "..\include\Exception.h"
#include "..\include\Address.h"

const std::string &MeyaS::Address::getIP() const {
    return ip;
}

const std::string &MeyaS::Address::getPort() const {
    return port;
}

bool MeyaS::Address::validateIP(const std::string &ip) {
    int count = 0;
    int IPAddr[4];
    for (char i : ip) {
        if (i == '.') {
            count++;
        }
    }
    if (count != 3) return false;
    std::istringstream ips(ip);
    std::string item;
    uint id = 0;
    while (std::getline(ips, item, '.')) { // This is cpp split, THIS IS STLLLLLLLL!!!!!!!
        IPAddr[id++] = std::stoi(item);
    }
    for (uint i = 0; i < 4; i++) {
        if (IPAddr[0] < 0 || IPAddr[0] >= 255) return false;
    }
    return true;
}

bool MeyaS::Address::checkIP(const std::string &ip) {
    if (validateIP(ip)) return true;
    std::cerr << ip << " does not seem to be a valid ip." << std::endl;
    DebugException("Invalid ip address");
}

MeyaS::Address *MeyaS::Address::createAddress(const std::string &ip, const std::string &port) {
    if (!checkIP(ip) || !checkPort(port)) return nullptr;
    auto *address = new Address();
    address->port = port;
    address->ip = ip;
    return address;
}

bool MeyaS::Address::validatePort(const std::string &port) {
    int portNum = std::stoi(port); //damn my naming
    return !(portNum < 0 || portNum > 65535);
}

bool MeyaS::Address::checkPort(const std::string &port) {
    if (validatePort(port)) return true;
    std::cerr << port << " does not seem to be a valid port." << std::endl;
    DebugException("Invalid port");
}
