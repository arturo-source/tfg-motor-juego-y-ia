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

    for(auto& pc: g.template getComponents<PerceptronComponent_t>()) {
        auto* phy = g.template getRequiredComponent<PhysicsComponent_t>(pc);
        if(!phy) continue;

        pc.keyUP_pressed   = isKeyPressed(pc, *phy, *ballPhy, PerceptronComponent_t::Up);
        pc.keyDOWN_pressed = isKeyPressed(pc, *phy, *ballPhy, PerceptronComponent_t::Down);
    }
}

template<typename GameCTX_t>
constexpr bool ArtificialInteligenceSystem_t<GameCTX_t>::isKeyPressed(const PerceptronComponent_t &pc, const PhysicsComponent_t& playerPhysics, const PhysicsComponent_t& ballPhysics, int8_t side) const {
    std::array<float, 8> dataInputs {
        1, playerPhysics.y, playerPhysics.vy, playerPhysics.aceleration,
        ballPhysics.x, ballPhysics.y, ballPhysics.vx, ballPhysics.vy
    };

    return pc.isKeyPressed(dataInputs, side);
}