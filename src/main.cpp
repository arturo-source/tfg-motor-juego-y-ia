//#include "tinyPTC/tinyptc.h" //Rutas de include del sistema: /usr, ...
// extern "C" {
//     #include <tinyPTC/src/linux/tinyptc.h> //Ruta relativa a la hora de compilar
// }
#include <iostream>
#include <game/man/state.hpp>
#include <game/man/game.hpp>

int main() {
    try {
        StateManager_t States;
        States.pushState<GameManager_t>(States);
        while (States.alive()) {
            States.update();
        }
    } catch(const std::exception& ex) {
        std::cout << "Capturada: " << ex.what() << "\n";
    }
}