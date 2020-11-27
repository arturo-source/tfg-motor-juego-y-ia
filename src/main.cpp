//#include "tinyPTC/tinyptc.h" //Rutas de include del sistema: /usr, ...
// extern "C" {
//     #include <tinyPTC/src/linux/tinyptc.h> //Ruta relativa a la hora de compilar
// }
#include <iostream>
#include <filesystem>
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
        std::vector<std::string> options;
        std::cin >> opt;
        switch (opt) {
            case 1: SM.pushState<GameManager_t>(SM); break;
            case 2: break;
            case 3: 
                do {
                    std::cout << "\nSelect one of the following:\n";
                    options = showFiles();
                    std::cout << " >> ";
                    std::cin >> opt;
                    if(opt >= options.size()) std::cout << "BAD ANSWER\n";
                } while(opt >= options.size()); //Select one inside the range

                SM.pushState<ReplayGame_t>(SM, options[opt]); 
                break;
            default: m_Alive = false;
        }
    }
    std::vector<std::string> showFiles() {
        std::vector<std::string> options {};
        uint pos = 0;
        for (const auto & entry : std::filesystem::directory_iterator("CSVs")) {
            std::cout << pos++ << ": " << entry.path() << "\n";
            options.push_back(entry.path());
        }
        return options;
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