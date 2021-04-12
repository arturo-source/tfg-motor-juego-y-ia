#pragma once
#include <ecs/util/typealiases.hpp>
#include <game/cmp/neuralnetwork.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/input.hpp>
#include <game/util/gamereferences.hpp>

template<typename GameCTX_t>
struct ArtificialInteligenceSystem_t {
    explicit ArtificialInteligenceSystem_t(GameReferences& gReferences) : gameReferences{gReferences}
    {}
 
    void update(GameCTX_t& g) const;
private:
    constexpr void keysPressed(NeuralNetwork_t& nn, const PhysicsComponent_t& phy, const InputComponent_t& inp, bool& keyUp, bool& keyDown) const noexcept;
    GameReferences& gameReferences;
};