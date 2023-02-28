#include "r/engine.h"
#include "r/keys.h"
#include "r/vector2.h"
#include "r/random.h"
#include "r/fov.h"

#include "entity.h"
#include "map.h"
#include "player.h"
#include "monster.h"

r::Engine engine({.minimal_screen_size{40, 30}, .title{"Roguelike Tutorial"}});


class MainState : public r::GameState
{
public:
    void runUpdate() {
        for(auto e : entities) {
            if (e) e->onUpdate();
        }
    }

    void onRender(r::frontend::Renderer& renderer) override {
        for(auto& t : map.tiles)
            t.visible = false;
        if (auto pl = player.lock()) {
            r::VisitFOV(pl->position, 8, [](r::ivec2 p) {
                map.tiles[p].visible = true;
                map.tiles[p].revealed = true;
                return map.tiles[p].type != Tile::Type::Wall;
            });
        }

        renderer.draw(r::Recti{{}, renderer.size()}, ' ', {1, 1, 1}, {0,0,0});
        for(auto p : r::Recti{{}, map.tiles.size()}) {
            auto glyph = ' ';
            auto fg = r::Color(1, 1, 1);
            auto bg = r::Color(0, 0, 0);
            switch(map.tiles[p].type) {
                case Tile::Type::Floor: glyph = '.'; fg = {0, 0.5, 0.5}; bg = {0, 0, 0}; break;
                case Tile::Type::Wall: glyph = '#'; fg = {0, 1, 0}, bg = {0, 0, 0}; break;
            }
            if (!map.tiles[p].visible)
                fg = {0.3, 0.3, 0.3};
            if (map.tiles[p].revealed)
                renderer.draw(p, glyph, fg, bg);
        }
        for(auto e : entities) {
            if (e && map.tiles[e->position].visible) renderer.draw(e->position, e->glyph, e->fg, e->bg);
        }
    }

    void onKey(int key) override {
        auto p = player.lock();
        if (!p) return;
        switch(key) {
        case r::KEY_LEFT: case 'h':  if (p->try_move({-1, 0})) runUpdate(); break;
        case r::KEY_RIGHT: case 'l': if (p->try_move({1, 0})) runUpdate(); break;
        case r::KEY_UP: case 'k':    if (p->try_move({0, -1})) runUpdate(); break;
        case r::KEY_DOWN: case 'j':  if (p->try_move({0, 1})) runUpdate(); break;
        }
    }
};

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    map.newMapRoomsAndCorridors();

    auto p = createEntity<Player>();
    p->position = map.rooms[0].center();
    player = p;

    for(auto n=1U; n<map.rooms.size(); n++) {
        auto type = r::chance(50) ? 'g' : 'o';
        auto monster = createEntity<Monster>(type);
        monster->position = map.rooms[n].center();
        if (type == 'o')
            monster->name = "Orc";
        else
            monster->name = "Goblin";
        monster->name += " #" + std::to_string(n);
    }

    engine.pushState<MainState>();
    engine.run();
    return 0;
}
