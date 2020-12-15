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
2. Train
3. Play against AI
4. Replay game
-. Exit
[[ SELECT OPTION ]])";
        int opt;
        std::cin >> opt;
        std::string filename;
        switch (opt) {
            case 1: SM.pushState<GameManager_t>(SM, AI); break;
            case 2:
                AI.train("CSVs/datainput0.csv", "CSVs/dataphysics0.csv");
                break;
            case 3: break;
            case 4: 
                filename = showOptions();
                SM.pushState<ReplayGame_t>(SM, filename); 
                break;
            default: m_Alive = false;
        }
    }

    std::string showOptions() {
        int opt;
        std::vector<std::string> options;
        
        do {
            std::cout << "\nSelect one of the following:\n";
            options = showFiles();
            std::cout << " >> ";
            if(options.size() == 0) {
                std::cout << "No files stored yet, first play a game\n";
                break;
            } else {
                std::cin >> opt;
                if(opt >= options.size()) std::cout << "BAD ANSWER\n";
            }
        } while(opt >= options.size()); //Select one inside the range
        
        return options[opt];
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
    ArtificialInteligenceSystem_t<ECS::EntityManager_t> AI;
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