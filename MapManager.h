#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <string>
#include <vector>

namespace Map
{

typedef std::vector<unsigned long> Shape;
typedef std::vector<int> Data;
typedef std::pair<int, int> Point;
typedef std::vector<Point> Path;

class MapManager
{
public:
    MapManager();
    virtual ~MapManager();

    const Shape& getShape() const;
    const Data& getData() const;

    void loadMap(const std::string& path);
    Path findOptimalPath(size_t x1, size_t y1, size_t x2, size_t y2);

private:
    Shape mShape;
    Data mData;

    bool isCoordinateValid(size_t x, size_t y) const;
    int getScalarCoordinateFrom2DCoordinate(size_t x, size_t y) const;
};

}

#endif