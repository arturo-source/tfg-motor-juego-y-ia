#include <game/sys/artificialinteligence.hpp>

template<typename GameCTX_t>
void ArtificialInteligenceSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    PhysicsComponent_t* ballPhy { nullptr };
    for(auto& col: g.template getComponents<ColliderComponent_t>()) {
        if(col.properties & ColliderComponent_t::P_IsBall) {
            ballPhy = g.template getRequiredComponent<PhysicsComponent_t>(col);
            break;
        }
    }
    if(!ballPhy) throw std::runtime_error("Ball physics missing in game.");

    for(auto& ic: g.template getComponents<InteligenceComponent_t>()) {
        auto* phy = g.template getRequiredComponent<PhysicsComponent_t>(ic);
        if(!phy) continue;

        ic.keyUP_pressed   = calculatedOutput(ic, *phy, *ballPhy, InteligenceComponent_t::Up);
        ic.keyDOWN_pressed = calculatedOutput(ic, *phy, *ballPhy, InteligenceComponent_t::Down);
    }
}

template<typename GameCTX_t>
constexpr bool ArtificialInteligenceSystem_t<GameCTX_t>::calculatedOutput(InteligenceComponent_t &ic, const PhysicsComponent_t& playerPhysics, const PhysicsComponent_t& ballPhysics, int8_t side) const {
    std::array<float, 10> dataInputs {
        1, playerPhysics.x, playerPhysics.y, playerPhysics.vx, playerPhysics.vy, playerPhysics.aceleration,
        ballPhysics.x, ballPhysics.y, ballPhysics.vx, ballPhysics.vy
    };

    return ic.calculateOutput(dataInputs, side);
}