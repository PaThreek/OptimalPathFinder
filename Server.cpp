#include "Server.h"

#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

namespace Server
{

constexpr int PORT = 8080;
constexpr int DATA_MAX_SIZE = 1024;
constexpr int ERROR_GETTING_PATH_RETURN_CODE = -1;
constexpr int SOCKET_CONNECTION_CREATION_ERROR_RESULT = -1;
constexpr int SOCKET_CONNECTION_CREATION_OK_RESULT = 0;

Server::Server(Map::MapManager& mapManager)
: mServerFD(-1)
, mConnectionFD(-1)
, mMapManager(mapManager)
{

}

Server::~Server()
{

}

void Server::serve()
{
    std::cout << "Starting Server" << std::endl;
    
    int socketConnectionResult = SOCKET_CONNECTION_CREATION_ERROR_RESULT;
    socketConnectionResult = createSocket();
    if (socketConnectionResult == SOCKET_CONNECTION_CREATION_OK_RESULT) {
        socketConnectionResult = acceptConnection();
    }

    if (socketConnectionResult == SOCKET_CONNECTION_CREATION_ERROR_RESULT) {
        std::cout << "Error creating socket connection. Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Waiting for Client requests." << std::endl;

    while (true)
    {
        // Wait for data
        int32_t buffer[DATA_MAX_SIZE] = {0};
        int bytesRead = readData(buffer);

        if (bytesRead > 0) {
            // Currently the only expected request from a client to the server is the one with the optimal path
            getOptimalPath(buffer[0], buffer[1], buffer[2], buffer[3]);
            printf("Coordinates: x1: %d y1: %d x2: %d y2: %d.\n", buffer[0], buffer[1], buffer[2], buffer[3]);
        }
    }
}

int Server::createSocket()
{
    std::cout << "Creating socket." << std::endl;

    // Creating socket file descriptor
    if ((mServerFD = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return ERROR_GETTING_PATH_RETURN_CODE;
    }

    // Attaching socket to the port
    int opt = 1;
    if (setsockopt(mServerFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        return ERROR_GETTING_PATH_RETURN_CODE;
    }

    mAddress.sin_family = AF_INET;
    mAddress.sin_addr.s_addr = INADDR_ANY;
    mAddress.sin_port = htons(PORT);

    // Binding socket to the specified address
    if (bind(mServerFD, (struct sockaddr *)&mAddress, sizeof(mAddress)) < 0) {
        return ERROR_GETTING_PATH_RETURN_CODE;
    }

    // Listening for incoming connections
    if (listen(mServerFD, 1) < 0) {
        return ERROR_GETTING_PATH_RETURN_CODE;
    }

    std::cout << "Socket created for port " << PORT << "." << std::endl;
    return SOCKET_CONNECTION_CREATION_OK_RESULT;
}

int Server::acceptConnection()
{
    std::cout << "Waiting for connection request." << std::endl;

    // Accepting incoming connection and returning the socket descriptor
    int addressLength = sizeof(mAddress);
    if ((mConnectionFD = accept(mServerFD, (struct sockaddr *)&mAddress, (socklen_t*)&addressLength)) < 0)
    {
        return ERROR_GETTING_PATH_RETURN_CODE;
    }

    std::cout << "Connection estabilished." << std::endl;
    return SOCKET_CONNECTION_CREATION_OK_RESULT;
}

int Server::readData(int32_t* buffer)
{       
    std::cout << "Waiting for the data to read from Client." << std::endl;

    // Reading data from the socket
    int bytesRead = read(mConnectionFD , buffer, DATA_MAX_SIZE);
    
    printf("Data read by Server: %d bytes.\n", bytesRead);

    return bytesRead;
}

void Server::writeData(int32_t* buffer)
{
    std::cout << "Writing data to the Client." << std::endl;
    
    // Sending data to the socket
    send(mConnectionFD, buffer, DATA_MAX_SIZE, 0);

    std::cout<< "Data sent to the Client." << std::endl;
}

void Server::getOptimalPath(int x1, int y1, int x2, int y2)
{
    Map::Path path = mMapManager.findOptimalPath(x1, y1, x2, y2);

    int32_t data[DATA_MAX_SIZE] = {0};
    // Write the count of coordinates in the beginning of the array. Value ERROR_GETTING_PATH_RETURN_CODE is error value.
    data[0] = (path.empty() ? ERROR_GETTING_PATH_RETURN_CODE : path.size() * 2);

    if (!path.empty()) {
        size_t i = 1;
        for (const auto& point : path) {
            data[i] = point.first;
            data[i+1] = point.second;
            i+=2;
        }
    }
    
    writeData(data);
}

}