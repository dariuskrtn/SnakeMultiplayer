#include <iostream>
#include <conio.h>
#include <network.h>
#include <windows.h>

using namespace std;

network *connection;

DWORD WINAPI chat(LPVOID lpParam)
{
    while (connection->isConnected()) {
        string unread = connection->receive();
        if (unread.substr(0, 3)=="msg")
        cout << unread.substr(3, unread.size()) << "\n";
    }
    return 0;
}

int main()
{
    connection = new network();
    if (!connection->initialize()) cout << "BLOGAI\n";
    cout << "Client or server?(c, s)\n";
    char n;
    cin >> n;
    if (n=='c') {
        string host, port;
        cout << "Client\n";
        cout << "Iveskite ip ir port: (ip port)\n";
        cin >> host >> port;
        if (connection->connectToServer((char*)host.c_str(), (char*)port.c_str())) {
            cout << "Connected";
        } else {
            cout << "Failed";
        }
    } else {
        cout << "Server\n";
        string port;
        cout << "Iveskite port:\n";
        cin >> port;
        if (connection->createServer((char*)port.c_str())) {
            cout << "Connected\n";
        } else {
            cout << "Failed";
        }
    }
    DWORD threadId;
    CreateThread(NULL, 0, chat, NULL, 0, &threadId);
    while (connection->isConnected()) {
        string tmp;
        getline(cin, tmp);
        string message = "msg"+tmp;
        connection->mail(message);
    }

    return 0;
}
