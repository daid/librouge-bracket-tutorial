#pragma once

#include "r/vector2.h"
#include "r/rect.h"

enum class TileType {
    Wall,
    Floor,
};

extern r::Vector2<TileType> map;

void newMapTest();

std::vector<r::Recti> newMapRoomsAndCorridors();
