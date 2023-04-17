#include "MapManager.h"

#include <fstream>
#include <queue>

#include "libnpy/include/npy.hpp"

namespace Map
{

MapManager::MapManager()
{
    loadMap("../maps/map.npy");
}

MapManager::~MapManager()
{

}

const Shape& MapManager::getShape() const
{
    return mShape;
}

const Data& MapManager::getData() const
{
    return mData;
}

void MapManager::loadMap(const std::string& path)
{
    bool fortran_order;
    Shape shape;
    Data data;

    try {
        npy::LoadArrayFromNumpy(path, shape, fortran_order, data);
        std::cout << "Map of size " << data.size() << " loaded" << std::endl;

        mShape = std::move(shape);
        mData = std::move(data);
    }
    catch (std::exception const& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
 
// check if the given cell is within the bounds of the grid
bool MapManager::isCoordinateValid(size_t x, size_t y) const
 {
    return x >= 0 && x < mShape[0] && y >= 0 && y < mShape[1];
}

int MapManager::getScalarCoordinateFrom2DCoordinate(size_t x, size_t y) const
{
    return x + y * mShape[1];
}
 
// perform breadth-first search to find the shortest path
Path MapManager::findOptimalPath(size_t x1, size_t y1, size_t x2, size_t y2)
{
    if (mShape.empty() || mData.empty()) {
        std::cout << "The map is empty!" << std::endl;
        return Path();
    }

    if (!isCoordinateValid(x1, y1) || !isCoordinateValid(x2, y2)) {
        std::cout << "Coordinates are not within the boundaries" << std::endl;
        return Path();
    }

    // directions for horizontal and vertical movement
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    std::queue<Point> q;
    
    Path parents;
    parents.resize(mData.size(), {-1, -1});

    std::vector<bool> visited;
    visited.resize(mData.size(), false);
    
    Data distance;
    distance.resize(mData.size(), 0);

    q.push({x1, y1});
    visited[getScalarCoordinateFrom2DCoordinate(x1, y1)] = true;
 
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
 
        if (x == x2 && y == y2) {

            Point end = {x, y};
            Path path;
            while(end.first != -1 && end.second != -1) {
                path.push_back(end);
                end = parents[getScalarCoordinateFrom2DCoordinate(end.first, end.second)];
            }
            reverse(path.begin(), path.end());

            std::cout << "The path consists of " << path.size() <<" points." << std::endl;
            return path;
        }
 
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
 
            int neigborScalarCoordinate = getScalarCoordinateFrom2DCoordinate(nx, ny);
            if (isCoordinateValid(nx, ny) && 
                mData[neigborScalarCoordinate] == 0 && 
                !visited[neigborScalarCoordinate]) {
                visited[neigborScalarCoordinate] = true;
                distance[neigborScalarCoordinate] = distance[getScalarCoordinateFrom2DCoordinate(x, y)] + 1;
                q.push({nx, ny});
                parents[neigborScalarCoordinate] = {x, y};
            }
        }
    }
 
    // end point is unreachable
    std::cout << "The end coordinate is unreachable." << std::endl;
    return Path();
}

}