# socket_programming

Overview
Chat Application with Single and Multi-Client Support

This project implements a chat application that allows for both a simple single server-client interaction as well as a more complex multi-client server scenario. The application uses TCP sockets for communication, and it's implemented in C++ using threading for handling multiple clients.
Features

    Single Server-Client Interaction:
        A single server communicates with one client.
        Both the server and client can send and receive messages.
        The connection can be gracefully terminated by either party by sending a "bye" message.

    Multi-Client Server:
        The server can accept multiple clients simultaneously.
        Each client runs in a separate thread, allowing for independent communication.
        Clients can disconnect individually by sending "bye".
        If the server sends "bye", all connected clients will be disconnected.

Architecture

    Server:
        Listens for incoming connections.
        Spawns a new thread for each client connection.
        Maintains a list of active client sockets.
        Uses mutexes for thread-safe access to the list of clients.
        Handles messages from clients and the server itself.

    Client:
        Connects to the server and sends messages.
        Listens for responses from the server.
        Can gracefully disconnect by sending a "bye" message.

    Communication:
        Clients can send messages to the server.
        The server can reply to clients.
        To terminate a connection, either the server or client can send "bye".

Example Scenarios

    Single Client:
        A single client connects to the server, sends messages, and receives replies until either party sends "bye".

    Multiple Clients:
        Multiple clients connect to the server. Each client communicates independently. The server can send a message to all clients or terminate all connections by sending "bye".
