#pragma once

#include "r/vec.h"
#include "r/color.h"
#include <memory>
#include <vector>

class Entity;
extern std::vector<std::shared_ptr<Entity>> entities;
template<typename T> auto createEntity() { auto result = std::make_shared<T>(); entities.push_back(result); return result; }

class Entity {
public:
    r::ivec2 position{};

    int glyph = 0;
    r::Color fg{1,1,1};
    r::Color bg{0,0,0};

    virtual void onUpdate() {}
};
