#include "r/engine.h"

r::Engine engine({.minimal_screen_size{40, 30}, .title{"Roguelike Tutorial"}});

class MainState : public r::GameState
{
public:
    void onRender(r::frontend::Renderer& renderer) override {
        renderer.draw(r::Recti{{}, renderer.size()}, ' ', {1, 1, 1}, {0,0,0});
        renderer.print({1, 1}, "Hello C++ World");
    }
};

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    engine.pushState<MainState>();
    engine.run();
    return 0;
}
