//#include "tinyPTC/tinyptc.h" //Rutas de include del sistema: /usr, ...
extern "C" {
    #include <tinyPTC/src/linux/tinyptc.h> //Ruta relativa a la hora de compilar
}
#include <cstdint>
#include <memory>
#include <iostream>
#include <thread>
#include <game/sys/render.tpp>
#include <game/sys/physics.tpp>
#include <game/sys/input.tpp>
#include <game/sys/collision.tpp>
#include <game/sys/health.tpp>
#include <game/sys/spawn.tpp>
#include <game/cmp/collider.hpp>
#include <game/util/gameobjectfactory.hpp>
#include <game/util/timer.hpp>
#include <ecs/man/entitymanager.tpp>

constexpr uint32_t kSCRWIDTH  {640};
constexpr uint32_t kSCRHEIGHT {360};
constexpr uint64_t FPS  { 60 };
constexpr uint64_t NSPF { 1000000000/FPS };

int main() {
    try {
        // Systems
        const RenderSystem_t<ECS::EntityManager_t> Render{kSCRWIDTH, kSCRHEIGHT};
        PysicsSystem_t<ECS::EntityManager_t> Physics;
        CollisionSystem_t<ECS::EntityManager_t> Collision{kSCRWIDTH, kSCRHEIGHT};
        InputSystem_t<ECS::EntityManager_t> Input;
        SpawnSystem_t<ECS::EntityManager_t> Spawn;
        const HealthSystem_t<ECS::EntityManager_t> Health;
        
        // Entities
        ECS::EntityManager_t EntityMan;
        GameObjectFactory_t GOFact { EntityMan };
        // GOFact.createPalette();
        // GOFact.createPalette();
        // GOFact.createBall();
        // GOFact.createEnemy(50, 200);
        // GOFact.createEnemy(40, 20);
        // GOFact.createSpawner(200, 1, 
        //     [&](const SpawnerComponent_t& spw) {
        //         const auto* phy = EntityMan.getRequiredComponent<PhysicsComponent_t>(spw);
        //         if(!phy) return;
        //         auto& e = GOFact.createEnemy(phy->x, phy->y);
        //         auto* p = e.getComponent<PhysicsComponent_t>();
        //         if(!p) return;
        //         p->vx = -1; p->vy = 0;
        //     }
        // );
        // GOFact.createPlayer(30, 100);
        
        // Main Loop
        GameTimer_t timer;
        while( !Input.isEscPressed() ) {
            timer.timedCall("REN", [&](){ Render.update(EntityMan); } );
            timer.timedCall("INP", [&](){ Input.update(EntityMan); } );
            timer.timedCall("PHY", [&](){ Physics.update(EntityMan); } );
            timer.timedCall("COL", [&](){ Collision.update(EntityMan); } );
            timer.timedCall("HEA", [&](){ Health.update(EntityMan); } );
            timer.timedCall("SPW", [&](){ Spawn.update(EntityMan); } );

            std::cout << "[EXT]" << timer.waitUntil_ns(NSPF) << "\n";
            timer.start();
        }
    } catch(...) {
        std::cout << "Capturada: \n";
    }
}