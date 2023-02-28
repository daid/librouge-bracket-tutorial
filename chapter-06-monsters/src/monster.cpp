#include "monster.h"
#include "player.h"
#include "map.h"
#include "r/line.h"
#include <stdio.h>

Monster::Monster(int type)
{
    glyph = type;
    fg = {1, 0, 0};
}

void Monster::onUpdate()
{
    auto p = player.lock();
    if (!p) return;
    for(auto p : r::TraceLine(position, p->position)) {
        if (map.tiles[p].type == Tile::Type::Wall)
            return;
    }
    printf("%s shouts insults\n", name.c_str());
}