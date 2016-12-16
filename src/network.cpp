#include "../include/network.h"


network::network()
{
    connected = false;
    listens = false;
    host = false;
}

network::~network()
{
    //dtor
}

bool network::initialize()
{
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup klaida: %d\n", iResult);
        return false;
    }
    return true;
}

bool network::createServer(char* port)
{
    connected = false;
    host = true;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo klaida: %d\n", iResult);
        WSACleanup();
        return false;
    }

    return true;
}

bool network::startListening()
{
        // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket klaida: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }
    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind klaida: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen klaida: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }
    listens = true;
    return true;
}

void network::stopListening()
{
    listens = false;
    closesocket(ListenSocket);
}

bool network::addPendingConnections()
{
    SOCKET ConnectSocket = INVALID_SOCKET;
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(ListenSocket, &readSet);
    timeval timeout;
    timeout.tv_sec = 0;  // Zero timeout (poll)
    timeout.tv_usec = 0;
    if(select(ListenSocket, &readSet, NULL, NULL, &timeout) == 1)
    {
        ConnectSocket=accept(ListenSocket, NULL, NULL);
    } else {
        return false;
    }
    if (ConnectSocket == INVALID_SOCKET) {
        printf("accept klaida: %d\n", WSAGetLastError());
        return false;
    }

    u_long iMode=1;
	ioctlsocket(ConnectSocket,FIONBIO,&iMode);
	peers.push_back(ConnectSocket);
	return true;
}

bool network::connectToServer(char* host, char* port)
{
    host = false;
    connected = false;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(host, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo klaida: %d\n", iResult);
        WSACleanup();
        return false;
    }
    SOCKET ConnectSocket = INVALID_SOCKET;
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket klaida: %d\n", WSAGetLastError());

            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }
    u_long iMode=1;
	ioctlsocket(ConnectSocket,FIONBIO,&iMode);
    peers.push_back(ConnectSocket);
    connected = true;
    return true;
}

bool network::mail(std::string message)
{
    if (peers.size()==0) return false;
    char *sendbuf = (char*)message.c_str();
    for (int i=0; i<peers.size(); i++) {
    iResult = send(peers[i], sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == -1) {
        printf("Vartotojas atsijunge\n");
        peers.erase(peers.begin()+i);
        i--;
    }

    }
    return true;
}

std::string network::receive()
{
        for (int i=0; i<peers.size(); i++) {
        char recvbuf[DEFAULT_BUFLEN];
        memset(recvbuf, 0, sizeof(recvbuf));
        int recvbuflen = DEFAULT_BUFLEN;
        iResult = recv(peers[i], recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            std::string message = std::string(recvbuf, iResult);
            if (host) mail(message);
            return message;
        }
        else if (iResult == 0) {
            printf("Connection closed\n");
            quit();
            return "cls";
        } else {
            //printf("recv failed with error: %d\n", WSAGetLastError());
            continue;
        }
        }
        return "err";
}

bool network::quit()
{
    connected = false;
    for (int i=0; i<peers.size(); i++) {
    iResult = shutdown(peers[i], SD_SEND);
    if (iResult != 0) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(peers[i]);
        WSACleanup();
        return false;
    }
    closesocket(peers[i]);
    WSACleanup();
    }
    return true;
}
