#pragma once

#include "r/vector2.h"
#include "r/rect.h"


struct Tile {
    enum class Type {
        Wall,
        Floor,
    } type = Type::Wall;
    bool visible = false;
    bool revealed = false;
};
class Map {
public:
    r::Vector2<Tile> tiles;
    std::vector<r::Recti> rooms;

    void applyHorizontalTunnel(int x1, int x2, int y);
    void applyVerticalTunnel(int y1, int y2, int x);
    void newMapRoomsAndCorridors();
};
extern Map map;
