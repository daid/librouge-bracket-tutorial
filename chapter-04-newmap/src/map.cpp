#include "map.h"
#include "r/random.h"

r::Vector2<TileType> map;

void newMapTest() {
    map.resize({40, 30}, TileType::Floor);

     // Make the boundaries walls
    for(int x=0; x<map.size().x; x++) {
        map[{x, 0}] = TileType::Wall;
        map[{x, map.size().y-1}] = TileType::Wall;
    }
    for(int y=0; y<map.size().y; y++) {
        map[{0, y}] = TileType::Wall;
        map[{map.size().x - 1, y}] = TileType::Wall;
    }

    // Now we'll randomly splat a bunch of walls. It won't be pretty, but it's a decent illustration.
    for(int i=0; i<400; i++) {
        auto x = r::irandom(1, map.size().x - 2);
        auto y = r::irandom(1, map.size().y - 2);
        if (x != 20 || y != 15)
            map[{x, y}] = TileType::Wall;
    }
}

void applyHorizontalTunnel(int x1, int x2, int y) {
    if (x1 > x2) std::swap(x1, x2);
    for(auto x=x1; x <= x2; x++) {
        map[{x, y}] = TileType::Floor;
    }
}

void applyVerticalTunnel(int y1, int y2, int x) {
    if (y1 > y2) std::swap(y1, y2);
    for(auto y=y1; y <= y2; y++) {
        map[{x, y}] = TileType::Floor;
    }
}

std::vector<r::Recti> newMapRoomsAndCorridors() {
    map.resize({40, 30}, TileType::Wall);

    std::vector<r::Recti> rooms;
    const auto MAX_ROOMS = 30;
    const auto MIN_SIZE = 4;
    const auto MAX_SIZE = 8;

    for(int n=0; n<MAX_ROOMS; n++) {
        auto w = r::irandom(MIN_SIZE, MAX_SIZE);
        auto h = r::irandom(MIN_SIZE, MAX_SIZE);
        auto x = r::irandom(1, map.size().x - w - 1) - 1;
        auto y = r::irandom(1, map.size().y - h - 1) - 1;
        auto new_room = r::Recti(x, y, w, h);
        auto ok = true;
        for(auto& other_room : rooms) {
            if (new_room.overlaps(other_room)) { ok = false; }
        }
        if (ok) {
            for(auto p : new_room.shrink({1, 1}))
                map[p] = TileType::Floor;

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

    return rooms;
}