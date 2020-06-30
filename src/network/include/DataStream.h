#pragma once

#include "DataSocket.h"

namespace MeyaS {
    // A socket capable of sending and receiving data
    class DataStream {
    public:
        DataStream() = delete;

        virtual ~DataStream();

        explicit DataStream(DataSocket *socket);

        //Set max wait time for recv and getLine
        void setWaitTime(int time);

        //Send raw data pack
        bool send(const DataPack &data);

        //Recv raw data
        DataPack *recv(uint maxLength);

        //Send a line of data, append delimiter to the end of s, return true if no error occurs.
        bool sendLine(std::string s, char delimiter = '$');

        //Send a line of data, append delimiter to the end of s, return true if no error occurs.
        bool sendLineW(std::wstring s, wchar_t delimiter = '$');

        //Receive a line of data ended with delimiter. If no full line of data is received within
        //maxWaitTime, return an empty string.
        std::string getLine(char delimiter = '$');

        //Receive a line of data ended with delimiter. If no full line of data is received within
        //maxWaitTime, return an empty string.
        std::wstring getLineW(wchar_t delimiter = '$');

        //The name of the worker, edit this to identify different connections
        std::wstring identifier;
    private:
        std::string cache;
        std::wstring cacheW;
        int maxWaitTime;
        DataSocket *socket;
    };
}