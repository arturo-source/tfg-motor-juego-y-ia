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
#include <game/cmp/inteligence.hpp>
#include <game/cmp/weapon.hpp>

struct GameObjectFactory_t {
    explicit GameObjectFactory_t(ECS::EntityManager_t& EntityMan) :
    m_EntMan(EntityMan)
    {}
    
    ECS::Entity_t& createPalette(uint8_t side, uint32_t color) const;
    ECS::Entity_t& createMinion(uint8_t side, uint32_t color) const;
    ECS::Entity_t& createBall(const uint32_t color) const;
    ECS::Entity_t& createBullet(const PhysicsComponent_t& palette_phy, uint8_t side) const;
    void           createWalls(const uint32_t scrWidth, const uint32_t scrHeight, const uint32_t columns, const uint32_t leftTeamColor, const uint32_t rightTeamColor) const;
    void           createMiddleLine(const uint32_t scrWidth, const uint32_t scrHeight) const;
    void           addInteligence(ECS::Entity_t& entity) const;
private:
    ECS::EntityManager_t& m_EntMan;
    ECS::Entity_t& createEntity(uint32_t w, uint32_t h, uint32_t color) const;
    ECS::Entity_t& createPalette(uint32_t w, uint32_t h, uint32_t color, uint8_t side) const;
    void           createWallsColumn(const uint32_t x, const uint32_t wallsPerColumn, const uint32_t wallWidth, const uint32_t wallHeight, const uint32_t color) const;
    constexpr static uint32_t paletteW { 10 }, paletteH { 100 };
};
