//#include "tinyPTC/tinyptc.h" //Rutas de include del sistema: /usr, ...
extern "C" {
    #include <tinyPTC/src/tinyptc.h> //Ruta relativa a la hora de compilar
}
#include <cstdint>
#include <memory>
#include <iostream>
#include <sys/render.hpp>
#include <sys/physics.hpp>
#include <sys/input.hpp>
#include <sys/collision.hpp>
#include <man/entitymanager.hpp>

constexpr uint32_t kSCRWIDTH  {640};
constexpr uint32_t kSCRHEIGHT {360};

int main() {
    try {
        // Systems
        const ECS::RenderSystem_t Render{kSCRWIDTH, kSCRHEIGHT};
        ECS::PysicsSystem_t Physics;
        ECS::CollisionSystem_t Collision;
        ECS::InputSystem_t Input;
        
        // Entities
        ECS::EntityManager_t EntityMan;
        EntityMan.createEntity(50, 200, "assets/character.png");
        EntityMan.createEntity(20, 40, "assets/character.png");
        auto& e = EntityMan.createEntity(30, 100, "assets/character.png");
        EntityMan.addInputComponent(e);
        
        // Main Loop
        while(Render.update(EntityMan)) {
            Input.update(EntityMan);
            Physics.update(EntityMan);
            Collision.update(EntityMan);
        }
    } catch(...) {
        std::cout << "Capturada: \n";
    }
}