#include <game/sys/physics.hpp>
#include <game/cmp/physics.hpp>

template<typename GameCTX_t>
bool PysicsSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& phy: g.template getComponents<PhysicsComponent_t>()) {
        phy.vy += phy.aceleration;
        phy.vy *= phy.friction;
        //Keep velocity variable between min and max
        phy.vy = std::clamp(phy.vy, phy.KMinVy, phy.KMaxVy);

        phy.x += phy.vx;
        phy.y += phy.vy;

        auto* inp = g.template getRequiredComponent<InputComponent_t>(phy);
    }
    return true;
}