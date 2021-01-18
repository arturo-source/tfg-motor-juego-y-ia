#include <game/sys/artificialinteligence.hpp>

template<typename GameCTX_t>
void ArtificialInteligenceSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& pc: g.template getComponents<PerceptronComponent_t>()) {
        auto* phy = g.template getRequiredComponent<PhysicsComponent_t>(pc);
        if(!phy) continue;

        bool keyUP_pressed   = isKeyPressed(pc, *phy, PerceptronComponent_t::Up);
        bool keyDOWN_pressed = isKeyPressed(pc, *phy, PerceptronComponent_t::Down);
        if( keyDOWN_pressed && !keyUP_pressed) phy->aceleration =  0.44;
        if(!keyDOWN_pressed &&  keyUP_pressed) phy->aceleration = -0.44;
    }
}

template<typename GameCTX_t>
constexpr bool ArtificialInteligenceSystem_t<GameCTX_t>::isKeyPressed(const PerceptronComponent_t &pc, const PhysicsComponent_t& playerPhysics, int8_t side) const {
    if(!ballPhysics) throw std::runtime_error("Ball physics missing in game.");
    std::array<float, 8> dataInputs {
        1, playerPhysics.y, playerPhysics.vy, playerPhysics.aceleration,
        ballPhysics->x, ballPhysics->y, ballPhysics->vx, ballPhysics->vy
    };

    return pc.isKeyPressed(dataInputs, side);
}