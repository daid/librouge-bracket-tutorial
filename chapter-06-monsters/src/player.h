#pragma once

#include "entity.h"

class Player : public Entity {
public:
    Player();

    bool try_move(r::ivec2 offset);
};
extern std::weak_ptr<Player> player;
