#pragma once

#include <vector>
#include "Socket.h"

namespace MeyaS {
    struct DataPack {
        DataPack() = delete;
        DataPack(void* data,uint length);
        explicit DataPack(const std::string& data);
        explicit DataPack(const std::wstring& data);
        ~DataPack();
        const byte *data;
        uint length;
        uint type{};
    };

    class DataSocket : public Socket {
    public:
        // Notice that recv shall not block. send may block is the buffer is full.
        MeyaS::DataPack *recv(uint maxLength);

        bool send(const DataPack &dataPack);

    protected:
        DataSocket() = default;
    };
}