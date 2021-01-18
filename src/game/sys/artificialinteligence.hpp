#pragma once
#include <ecs/util/typealiases.hpp>
#include <game/cmp/inteligence.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/input.hpp>
#include <game/cmp/collider.hpp>

//Forward declaration
namespace ECS {
    struct Keyboard_t;
}

template<typename GameCTX_t>
struct ArtificialInteligenceSystem_t {
    explicit ArtificialInteligenceSystem_t() = default;

    void update(GameCTX_t& g) const;
    
    PhysicsComponent_t* ballPhysics { nullptr };
private:
    constexpr bool isKeyPressed(const PerceptronComponent_t &pc, const PhysicsComponent_t& playerPhysics, int8_t side) const;
};