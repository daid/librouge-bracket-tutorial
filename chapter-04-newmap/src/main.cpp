#include "r/engine.h"
#include "r/keys.h"
#include "r/vector2.h"
#include "r/random.h"

#include "entity.h"
#include "map.h"
#include "player.h"

r::Engine engine({.minimal_screen_size{40, 30}, .title{"Roguelike Tutorial"}});


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
        case r::KEY_LEFT: case 'h': p->try_move({-1, 0}); break;
        case r::KEY_RIGHT: case 'l': p->try_move({1, 0}); break;
        case r::KEY_UP: case 'k': p->try_move({0, -1}); break;
        case r::KEY_DOWN: case 'j': p->try_move({0, 1}); break;
        }
    }
};

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    auto rooms = newMapRoomsAndCorridors();

    auto p = createEntity<Player>();
    p->position = rooms[0].center();
    player = p;

    engine.pushState<MainState>();
    engine.run();
    return 0;
}
