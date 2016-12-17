#ifndef NETWORK_H
#define NETWORK_H
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501

#include <ws2tcpip.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

#define DEFAULT_BUFLEN 512

class network
{
    public:
        network();
        virtual ~network();
        bool initialize();
        bool createServer(char* port);
        bool connectToServer(char* host, char* port);
        bool mail(std::string message);
        std::string receive();
        bool quit();
        bool startListening();
        void stopListening();
        bool addPendingConnections();
        int countPeers();
        bool connected;
        bool listens;
        bool host;
    protected:

    private:
        WSADATA wsaData;
        int iResult;
        SOCKET ListenSocket = INVALID_SOCKET;
        struct addrinfo *result = NULL, *ptr = NULL, hints;
        std::vector <SOCKET> peers;
};

#endif // NETWORK_H
