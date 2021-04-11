#pragma once
#include <ecs/util/typealiases.hpp>
#include <game/cmp/inteligence.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/input.hpp>
#include <game/cmp/collider.hpp>
#include <game/util/gamereferences.hpp>

template<typename GameCTX_t>
struct ArtificialInteligenceSystem_t {
    explicit ArtificialInteligenceSystem_t(GameReferences& gReferences) : gameReferences{gReferences}
    {}
 
    void update(GameCTX_t& g) const;
private:
    constexpr bool isKeyPressed(const PerceptronComponent_t &pc, const PhysicsComponent_t& playerPhysics, int8_t side) const;
    GameReferences& gameReferences;
};