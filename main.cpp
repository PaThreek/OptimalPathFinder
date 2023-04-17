#include <iostream>
#include <memory>

#include "Server.h"

#include "MapManager.h"

int main(int argc, char** argv)
{
    std::unique_ptr<Map::MapManager> mapManager = std::make_unique<Map::MapManager>();
    std::unique_ptr<Server::Server> server = std::make_unique<Server::Server>(*mapManager.get());
    server->serve();

    return 0;
}