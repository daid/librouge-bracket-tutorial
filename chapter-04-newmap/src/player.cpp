#include "player.h"
#include "map.h"

std::weak_ptr<Player> player;

Player::Player() {
    glyph = '@';
    fg = {1, 1, 0.2};
}

void Player::try_move(r::ivec2 offset) {
    auto new_position = r::ivec2{
        std::clamp(position.x + offset.x, 0, 39),
        std::clamp(position.y + offset.y, 0, 29)
    };
    if (map[new_position] != TileType::Wall)
        position = new_position;
}
