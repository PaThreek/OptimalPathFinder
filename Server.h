#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

#include "MapManager.h"


namespace Server
{

class Server
{
public:
    Server(Map::MapManager& mapManager);
    virtual ~Server();

    void serve();

private:
    int createSocket();
    int acceptConnection();
    int readData(int32_t* buffer);
    void writeData(int32_t* buffer);

    void getOptimalPath(int x1, int y1, int x2, int y2);

    int mServerFD;
    int mConnectionFD;
    struct sockaddr_in mAddress;

    Map::MapManager& mMapManager;
};

}

#endif