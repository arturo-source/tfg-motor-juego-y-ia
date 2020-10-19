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
#include <game/cmp/collider.hpp>
#include <ecs/man/entitymanager.hpp>

constexpr uint32_t kSCRWIDTH  {640};
constexpr uint32_t kSCRHEIGHT {360};

ECS::Entity_t& createEntity(ECS::EntityManager_t& EntityMan, uint32_t x, uint32_t y, const std::string_view filename) {
    auto& e = EntityMan.createEntity(); // leer filename¡¡
    auto& rn = EntityMan.addComponent<RenderComponent_t>(e);
    auto& ph = EntityMan.addComponent<PhysicsComponent_t>(e);
    auto& cl = EntityMan.addComponent<ColliderComponent_t>(e);
    rn.loadFromFile(filename);
    ph.x = x; ph.y = y;
    cl.box.xLeft  = 0;
    cl.box.xRight = rn.w;
    cl.box.yUp    = 0;
    cl.box.yDown  = rn.h;

    return e;
}

void createPlayer(ECS::EntityManager_t& EntityMan, uint32_t x, uint32_t y) {
    auto& e = createEntity(EntityMan, x, y, "assets/character.png");
    EntityMan.addComponent<InputComponent_t>(e);
}

void createEnemy(ECS::EntityManager_t& EntityMan, uint32_t x, uint32_t y) {
    auto& e = createEntity(EntityMan, x, y, "assets/enemy.png");
}

int main() {
    try {
        // Systems
        const RenderSystem_t<ECS::EntityManager_t> Render{kSCRWIDTH, kSCRHEIGHT};
        PysicsSystem_t<ECS::EntityManager_t> Physics;
        CollisionSystem_t<ECS::EntityManager_t> Collision{kSCRWIDTH, kSCRHEIGHT};
        InputSystem_t<ECS::EntityManager_t> Input;
        
        // Entities
        ECS::EntityManager_t EntityMan;
        createEnemy(EntityMan, 50, 200);
        createEnemy(EntityMan, 40, 20);
        createPlayer(EntityMan, 30, 100);
        
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