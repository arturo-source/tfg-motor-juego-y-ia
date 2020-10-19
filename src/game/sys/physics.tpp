#include <game/sys/physics.hpp>
#include <game/cmp/physics.hpp>

template<typename GameCTX_t>
bool PysicsSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& phy: g.template getComponents<PhysicsComponent_t>()) {
        phy.x += phy.vx;
        phy.y += phy.vy;
    }
    return true;
}
