#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

#define MAX 500
#define PORT 5200

// Global vector to keep track of client sockets
vector<int> clientSockets;
mutex mtx; // Mutex for thread-safe access to clientSockets

void handleClient(int clientSocket, int clientIndex) {
    char buff[MAX];
    while (true) {
        int rMsgSize = recv(clientSocket, buff, MAX, 0);
        if (rMsgSize > 0) {
            cout << "Client[" << clientIndex << "]: " << buff << endl;

            // Check for 'bye' message from the client
            if (buff[0] == 'b' && buff[1] == 'y' && buff[2] == 'e') {
                cout << "Server: Client[" << clientIndex << "] disconnected." << endl;

                // Lock mutex to safely modify clientSockets
                mtx.lock();
                close(clientSocket); // Close the socket for this client
                clientSockets.erase(remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
                mtx.unlock();
                break;
            }

            cout << "Server: ";
            string s;
            getline(cin, s);

            // If server says 'bye', disconnect all clients
            if (s == "bye") {
                cout << "Server: Bye! Disconnecting all clients..." << endl;

                // Lock mutex to safely modify clientSockets
                mtx.lock();
                for (int sock : clientSockets) {
                    send(sock, s.c_str(), s.size() + 1, 0);
                    close(sock); // Close the client socket
                }
                clientSockets.clear(); // Clear the list of client sockets
                mtx.unlock();
                break; // Exit the loop to end server thread
            }

            // Send the server message to the current client
            send(clientSocket, s.c_str(), s.size() + 1, 0);
        }
    }
}

int main() {
    int sSHandler = socket(AF_INET, SOCK_STREAM, 0);
    if (sSHandler < 0) {
        cout << "Unable to create SERVER SOCKET." << endl;
        return 0;
    }

    struct sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(PORT);
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sSHandler, (struct sockaddr*)&serAddr, sizeof(serAddr)) < 0) {
        cout << "Server Socket Connection failed" << endl;
        return 0;
    }

    if (listen(sSHandler, 5) < 0) {
        cout << "Listener has failed" << endl;
        return 0;
    }

    cout << "\t\t\t…Waiting for connections… \n\n\n";
    int clientIndex = 0;

    while (true) {
        struct sockaddr_in clAddr;
        socklen_t len = sizeof(clAddr);
        int cSHandler = accept(sSHandler, (struct sockaddr*)&clAddr, &len);

        if (cSHandler < 0) {
            cout << "Server avoided the connection request." << endl;
            continue;
        }

        cout << "Server accepted the connection request from client[" << clientIndex << "]" << endl;

        // Lock mutex to safely modify clientSockets
        mtx.lock();
        clientSockets.push_back(cSHandler); // Add new client socket
        mtx.unlock();

        // Create a new thread to handle the client
        thread(handleClient, cSHandler, clientIndex++).detach();
    }

    close(sSHandler);
    return 0;
}

