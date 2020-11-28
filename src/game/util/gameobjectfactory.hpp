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
    
    ECS::Entity_t& createEntity(float x, float y, uint32_t w, uint32_t h, uint32_t color) const;
    ECS::Entity_t& createPalette(float x, float y, uint8_t side) const;
    ECS::Entity_t& createBall(float x, float y) const;
private:
    ECS::EntityManager_t& m_EntMan;
};
