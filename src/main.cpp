//#include "tinyPTC/tinyptc.h" //Rutas de include del sistema: /usr, ...
// extern "C" {
//     #include <tinyPTC/src/linux/tinyptc.h> //Ruta relativa a la hora de compilar
// }
#include <iostream>
#include <game/man/state.hpp>
#include <game/man/menu.hpp>

int main() {
    std::filesystem::create_directory("dataset_CSVs");
    std::filesystem::create_directory("weights_CSVs");
    constexpr uint32_t kSCRWIDTH  {900};
    constexpr uint32_t kSCRHEIGHT {500};
    
    const RenderSystem_t<ECS::EntityManager_t> Render{kSCRWIDTH, kSCRHEIGHT};
    InputSystem_t<ECS::EntityManager_t> Input;

    try {
        StateManager_t States;
        States.pushState<MainMenu_t>(States, Render, Input, kSCRWIDTH, kSCRHEIGHT);
        while (States.alive()) {
            States.update();
        }
    } catch(const std::exception& ex) {
        std::cout << "Capturada: " << ex.what() << "\n";
    }
}