#include "r/engine.h"
#include "r/keys.h"

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

class LeftMover : public Entity {
public:
    LeftMover() { glyph = '@'; fg = {1, 0, 0}; }
    virtual void onUpdate() {
        position.x = position.x - 1;
        if (position.x < 0) position.x = 39;
    }
};

class Player : public Entity {
public:
    Player() { glyph = '@'; fg = {1, 1, 0.2}; }

    void try_move(r::ivec2 offset) {
        position.x = std::clamp(position.x + offset.x, 0, 39);
        position.y = std::clamp(position.y + offset.y, 0, 29);
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

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    auto p = createEntity<Player>();
    p->position = {20, 15};
    player = p;

    for(int i=0; i<10; i++)
        createEntity<LeftMover>()->position = {i * 4, 20};

    engine.pushState<MainState>();
    engine.run();
    return 0;
}
