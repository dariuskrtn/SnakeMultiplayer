#include <iostream>
#include <conio.h>
#include <network.h>
#include <windows.h>
#include <utilities.h>

using namespace std;

network *connection;

DWORD WINAPI checkForPending(LPVOID lpParam)
{
    while (connection->listens) {
        connection->addPendingConnections();
    }
    return 0;
}

int main()
{
    string name;
    string ip, port;
    char bodyChar, client;
    bool host = false;
    cout << "Welcome to Snake (Multiplayer edition)\n";
    cout << "Please write your name:\n";
    cin >> name;
    cout << "Now it's time to choose your body character:\n(it can be any non blank character, except '*' and '.')\n";
    cin >> bodyChar;
    cout << "Your snake will look like this: ";
    for (int i=0; i<3; i++) cout << bodyChar;
    cout << "\n";
    cout << "Are you a host or a client?(type 'h' or 'c')\n";
    cin >> client;
    host = client=='c'?false:true;

    connection = new network();
    if (!connection->initialize()) {
        cout << "Failed to initialize connection. Please try again.\n";
        return 0;
    }
    if (host) {
        cout << "Please enter connection port: (123)\n";
        cin >> port;
        if (!connection->createServer((char*)port.c_str()) || !connection->startListening()) {
            cout << "Failed to start server.\n Please try again.\n";
            return 0;
        }
        DWORD threadId;
        CreateThread(NULL, 0, checkForPending, NULL, 0, &threadId);
        system("cls");
        cout << "Waiting for others.\n";
        while (connection->countPeers()==0) {}
        int currPlayers = 0;
        while (!kbhit()) {
            if (currPlayers!=connection->countPeers()) {
                utilities::gotoxy(0, 3);
                cout << "Current players: " << connection->countPeers()+1 << "\n";
                cout << "Press any key to start or wait for more players to connect\n";
                utilities::gotoxy(0, 0);
                currPlayers = connection->countPeers();
            }
        }

    } else {
        cout << "Please enter server's ip and port: (127.0.0.1 123)\n";
        cin >> ip >> port;
        cout << "Connecting...\n";
        if (connection->connectToServer((char*)ip.c_str(), (char*)port.c_str())) {
            cout << "Successfully connected\n";
        } else {
            cout << "Failed to connect\n";
            return 0;
        }
    }
    cout << "BYE";
    return 0;
}
