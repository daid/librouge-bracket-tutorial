#include "map.h"
#include "r/random.h"

Map map;

void newMapTest() {
    map.tiles.resize({40, 30}, {Tile::Type::Floor});

     // Make the boundaries walls
    for(int x=0; x<map.tiles.size().x; x++) {
        map.tiles[{x, 0}].type = Tile::Type::Wall;
        map.tiles[{x, map.tiles.size().y-1}].type = Tile::Type::Wall;
    }
    for(int y=0; y<map.tiles.size().y; y++) {
        map.tiles[{0, y}].type = Tile::Type::Wall;
        map.tiles[{map.tiles.size().x - 1, y}].type = Tile::Type::Wall;
    }

    // Now we'll randomly splat a bunch of walls. It won't be pretty, but it's a decent illustration.
    for(int i=0; i<400; i++) {
        auto x = r::irandom(1, map.tiles.size().x - 2);
        auto y = r::irandom(1, map.tiles.size().y - 2);
        if (x != 20 || y != 15)
            map.tiles[{x, y}].type = Tile::Type::Wall;
    }
}

void Map::applyHorizontalTunnel(int x1, int x2, int y) {
    if (x1 > x2) std::swap(x1, x2);
    for(auto x=x1; x <= x2; x++) {
        tiles[{x, y}].type = Tile::Type::Floor;
    }
}

void Map::applyVerticalTunnel(int y1, int y2, int x) {
    if (y1 > y2) std::swap(y1, y2);
    for(auto y=y1; y <= y2; y++) {
        tiles[{x, y}].type = Tile::Type::Floor;
    }
}

void Map::newMapRoomsAndCorridors() {
    tiles.resize({40, 30}, {Tile::Type::Wall});

    const auto MAX_ROOMS = 30;
    const auto MIN_SIZE = 4;
    const auto MAX_SIZE = 8;

    for(int n=0; n<MAX_ROOMS; n++) {
        auto w = r::irandom(MIN_SIZE, MAX_SIZE);
        auto h = r::irandom(MIN_SIZE, MAX_SIZE);
        auto x = r::irandom(1, tiles.size().x - w - 1) - 1;
        auto y = r::irandom(1, tiles.size().y - h - 1) - 1;
        auto new_room = r::Recti(x, y, w, h);
        auto ok = true;
        for(auto& other_room : rooms) {
            if (new_room.overlaps(other_room)) { ok = false; }
        }
        if (ok) {
            for(auto p : new_room.shrink({1, 1}))
                tiles[p].type = Tile::Type::Floor;

            if (!rooms.empty()) {
                auto new_center = new_room.center();
                auto prev_center = rooms[rooms.size()-1].center();
                if (r::chance(50)) {
                    applyHorizontalTunnel(prev_center.x, new_center.x, new_center.y);
                    applyVerticalTunnel(prev_center.y, new_center.y, new_center.x);
                } else {
                    applyVerticalTunnel(prev_center.y, new_center.y, prev_center.x);
                    applyHorizontalTunnel(prev_center.x, new_center.x, new_center.y);
                }
            }
            
            rooms.push_back(new_room);
        }
    }
}