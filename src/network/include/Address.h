#pragma once

#include <string>

#include "common.h"

namespace MeyaS {
    class Address {
    public:

        const std::string & getIP() const;

        const std::string & getPort() const;

        static bool validateIP(const std::string &ip);
        static bool checkIP(const std::string &ip);

        static bool validatePort(const std::string& port);
        static bool checkPort(const std::string &port);

        static Address* createAddress(const std::string &ip,const std::string &port);

    private:
        Address() = default;
        std::string ip;
        std::string port;
    };
} // namespace MeyaS
