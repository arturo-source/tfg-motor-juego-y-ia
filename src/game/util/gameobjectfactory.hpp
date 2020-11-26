#pragma once
#include <string_view>
#include <ecs/man/entitymanager.tpp>
#include <game/cmp/collider.hpp>
#include <game/cmp/input.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/render.hpp>
#include <game/cmp/spawner.hpp>
#include <game/cmp/health.hpp>
#include <game/cmp/score.hpp>

struct GameObjectFactory_t {
    explicit GameObjectFactory_t(ECS::EntityManager_t& EntityMan) :
    m_EntMan(EntityMan)
     {}
    
    ECS::Entity_t& createEntity(uint32_t x, uint32_t y, const std::string_view filename) const;
    ECS::Entity_t& createEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) const;
    ECS::Entity_t& createPlayer(uint32_t x, uint32_t y) const;
    ECS::Entity_t& createEnemy(uint32_t x, uint32_t y) const;
    ECS::Entity_t& createPalette(uint32_t x, uint32_t y, bool isRight) const;
    ECS::Entity_t& createBall(uint32_t x, uint32_t y) const;

    template<typename CALLABLE_t>
    ECS::Entity_t& createSpawner(uint32_t x, uint32_t y, CALLABLE_t callback) const {
        auto& e   = m_EntMan.createEntity();
        //El componente de render y el de collider es temporal...
        //porque de momento no estoy clippeando los sprites y si 
        //spawnea un sprite fuera de pantalla, peta
        auto& rn = m_EntMan.addComponent<RenderComponent_t>(e);
        auto& spw = m_EntMan.addComponent<SpawnerComponent_t>(e);
        auto& ph  = m_EntMan.addComponent<PhysicsComponent_t>(e);
        auto& cl = m_EntMan.addComponent<ColliderComponent_t>(e);
        cl.mask = ColliderComponent_t::L_NoLayer; //Collide with nothing
        rn.loadFromFile("assets/enemy.png");
        ph.x = x; ph.y = y;
        ph.vy = 1;
        ph.gravity = 0;
        spw.spawnerMethod = callback;
        spw.to_be_spawned = 100;
        cl.box.xLeft  = 0;
        cl.box.xRight = rn.w;
        cl.box.yUp    = 0;
        cl.box.yDown  = rn.h;

        return e;
    }

private:
    ECS::EntityManager_t& m_EntMan;
};
