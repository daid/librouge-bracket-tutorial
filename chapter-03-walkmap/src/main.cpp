#include "r/engine.h"
#include "r/keys.h"
#include "r/vector2.h"
#include "r/random.h"

r::Engine engine({.minimal_screen_size{40, 30}, .title{"Roguelike Tutorial"}});

class Entity;
std::vector<std::shared_ptr<Entity>> entities;
template<typename T> auto createEntity() { auto result = std::make_shared<T>(); entities.push_back(result); return result; }

class Entity {
public:
    r::ivec2 position{};

    int glyph = 0;
    r::Color fg{1,1,1};
    r::Color bg{0,0,0};

    virtual void onUpdate() {}
};

enum class TileType {
    Wall,
    Floor,
};

r::Vector2<TileType> map;

class Player : public Entity {
public:
    Player() { glyph = '@'; fg = {1, 1, 0.2}; }

    void try_move(r::ivec2 offset) {
        auto new_position = r::ivec2{
            std::clamp(position.x + offset.x, 0, 39),
            std::clamp(position.y + offset.y, 0, 29)
        };
        if (map[new_position] != TileType::Wall)
            position = new_position;
    }
};
std::weak_ptr<Player> player;

class MainState : public r::GameState
{
public:
    void onFixedUpdate() override {
        for(auto e : entities) {
            if (e) e->onUpdate();
        }
    }

    void onRender(r::frontend::Renderer& renderer) override {
        renderer.draw(r::Recti{{}, renderer.size()}, ' ', {1, 1, 1}, {0,0,0});
        for(auto p : r::Recti{{}, map.size()}) {
            switch(map[p]) {
                case TileType::Floor: renderer.draw(p, '.', {0.5, 0.5, 0.5}, {0, 0, 0}); break;
                case TileType::Wall: renderer.draw(p, '#', {0, 1, 0}, {0, 0, 0}); break;
            }
        }
        for(auto e : entities) {
            if (e) renderer.draw(e->position, e->glyph, e->fg, e->bg);
        }
    }

    void onKey(int key) override {
        auto p = player.lock();
        if (!p) return;
        switch(key) {
        case r::KEY_LEFT: p->try_move({-1, 0}); break;
        case r::KEY_RIGHT: p->try_move({1, 0}); break;
        case r::KEY_UP: p->try_move({0, -1}); break;
        case r::KEY_DOWN: p->try_move({0, 1}); break;
        }
    }
};

void newMap() {
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

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    newMap();

    auto p = createEntity<Player>();
    p->position = {20, 15};
    player = p;

    engine.pushState<MainState>();
    engine.run();
    return 0;
}
