//#include "tinyPTC/tinyptc.h" //Rutas de include del sistema: /usr, ...
extern "C" {
    #include <tinyPTC/src/tinyptc.h> //Ruta relativa a la hora de compilar
}
#include <cstdint>
#include <memory>
#include <iostream>
#include <game/sys/render.tpp>
#include <game/sys/physics.tpp>
#include <game/sys/input.tpp>
#include <game/sys/collision.tpp>
#include <ecs/man/entitymanager.hpp>

constexpr uint32_t kSCRWIDTH  {640};
constexpr uint32_t kSCRHEIGHT {360};

ECS::Entity_t& createEntity(ECS::EntityManager_t& EntityMan, uint32_t x, uint32_t y, const std::string_view filename) {
    auto& e = EntityMan.createEntity(); // leer filename¡¡
    auto& rn = EntityMan.addComponent<RenderComponent_t>(e);
    auto& ph = EntityMan.addComponent<PhysicsComponent_t>(e);
    rn.loadFromFile(filename);
    ph.x = x; ph.y = y;

    return e;
}

int main() {
    try {
        // Systems
        const RenderSystem_t<ECS::EntityManager_t> Render{kSCRWIDTH, kSCRHEIGHT};
        PysicsSystem_t<ECS::EntityManager_t> Physics;
        CollisionSystem_t<ECS::EntityManager_t> Collision;
        InputSystem_t<ECS::EntityManager_t> Input;
        
        // Entities
        ECS::EntityManager_t EntityMan;
        createEntity(EntityMan, 50, 200, "assets/character.png");
        createEntity(EntityMan, 20, 40, "assets/character.png");
        auto& e = createEntity(EntityMan, 30, 100, "assets/character.png");
        EntityMan.addComponent<InputComponent_t>(e);
        
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