#include <iostream>
#include <conio.h>
#include <include/network.h>
#include <windows.h>

using namespace std;

network *connection;
string name;

DWORD WINAPI chat(LPVOID lpParam)
{
    while (connection->connected || connection->host) {
        string unread = connection->receive();
        if (unread.substr(0, 3)=="msg")
        cout << unread.substr(3, unread.size()) << "\n";
        if (connection->host && connection->addPendingConnections()) {
            cout << "Prisijunge naujas zmogus\n";
        }
    }
    return 0;
}

int main()
{
    connection = new network();
    if (!connection->initialize()) cout << "BLOGAI\n";
    cout << "Enter name:\n";
    cin >> name;
    cout << "Client or server?(c, s)\n";
    char n;
    cin >> n;
    if (n=='c') {
        string host="192.168.1.66", port="123";
        cout << "Client\n";
        cout << "Iveskite ip ir port: (ip port)\n";
        //cin >> host >> port;
        if (connection->connectToServer((char*)host.c_str(), (char*)port.c_str())) {
            cout << "Connected\n";
        } else {
            cout << "Failed\n";
        }
    } else {
        cout << "Server\n";
        string port="123";
        cout << "Iveskite port:\n";
        //cin >> port;
        if (connection->createServer((char*)port.c_str())) {
            connection->startListening();
        }
    }

    DWORD threadId;
    CreateThread(NULL, 0, chat, NULL, 0, &threadId);
    while (connection->connected || connection->listens) {
        string tmp;
        getline(cin, tmp);
        string message = "msg"+name+": "+tmp;
        connection->mail(message);
    }
    cout << "BYE";
    return 0;
}
