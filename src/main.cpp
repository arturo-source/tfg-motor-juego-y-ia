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
#include <game/sys/spawn.tpp>
#include <game/cmp/collider.hpp>
#include <game/util/gameobjectfactory.hpp>
#include <ecs/man/entitymanager.hpp>

constexpr uint32_t kSCRWIDTH  {640};
constexpr uint32_t kSCRHEIGHT {360};

int main() {
    try {
        // Systems
        const RenderSystem_t<ECS::EntityManager_t> Render{kSCRWIDTH, kSCRHEIGHT};
        PysicsSystem_t<ECS::EntityManager_t> Physics;
        CollisionSystem_t<ECS::EntityManager_t> Collision{kSCRWIDTH, kSCRHEIGHT};
        InputSystem_t<ECS::EntityManager_t> Input;
        SpawnSystem_t<ECS::EntityManager_t> Spawn;
        
        // Entities
        ECS::EntityManager_t EntityMan;
        GameObjectFactory_t GOFact { EntityMan };
        GOFact.createEnemy(50, 200);
        GOFact.createEnemy(40, 20);
        GOFact.createSpawner(200, 1, 
            [&](const SpawnerComponent_t& spw) {
                const auto* esp = EntityMan.getEntityByID(spw.getEntityID());
                if(!esp) return;
                const auto* phy = esp->getComponent<PhysicsComponent_t>();
                if(!phy) return;
                [[maybe_unused]]auto& e = GOFact.createEnemy(phy->x, phy->y);
                
            }
        );
        GOFact.createPlayer(30, 100);
        
        // Main Loop
        while(Render.update(EntityMan)) {
            Input.update(EntityMan);
            Physics.update(EntityMan);
            Collision.update(EntityMan);
            Spawn.update(EntityMan);
        }
    } catch(...) {
        std::cout << "Capturada: \n";
    }
}