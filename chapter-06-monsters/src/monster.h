#pragma once

#include "entity.h"
#include <string>


class Monster : public Entity
{
public:
    Monster(int type);

    void onUpdate() override;

    std::string name;
};