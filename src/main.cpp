//#include "tinyPTC/tinyptc.h" //Rutas de include del sistema: /usr, ...
// extern "C" {
//     #include <tinyPTC/src/linux/tinyptc.h> //Ruta relativa a la hora de compilar
// }
#include <iostream>
#include <game/man/game.hpp>
#include <game/man/state.hpp>
#include <game/man/replay.hpp>

struct MenuState_t : StateBase_t {
    explicit MenuState_t(StateManager_t& sm) : SM{sm} {}
    void update() final {
        std::cout << R"(
MENU:
1. Play
2. Options
3. Replay game
-. Exit
[[ SELECT OPTION ]])";
        int opt;
        std::cin >> opt;
        switch (opt) {
            case 1: SM.pushState<GameManager_t>(SM); break;
            case 2:  break;
            case 3: SM.pushState<ReplayGame_t>(SM, "datainput0.bin"); break;
            default: m_Alive = false;
        }
    }
    bool alive() final { return m_Alive; }
private:
    bool m_Alive { true };
    StateManager_t& SM;
};


int main() {
    try {
        StateManager_t States;
        States.pushState<MenuState_t>(States);
        while (States.alive()) {
            States.update();
        }
    } catch(...) {
        std::cout << "Capturada: \n";
    }
}