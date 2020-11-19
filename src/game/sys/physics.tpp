#include <game/sys/physics.hpp>
#include <game/cmp/physics.hpp>

template<typename GameCTX_t>
bool PysicsSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& phy: g.template getComponents<PhysicsComponent_t>()) {
        //Jump
        if( phy.jumpIdx < phy.jumpTable.size() ) {
            phy.vy += phy.jumpTable[phy.jumpIdx]; 
            ++phy.jumpIdx;
        }

        //Gravity
        phy.vy += phy.gravity;
        //Mantener variable entre un máx y un mín
        phy.vy = std::clamp(phy.vy, phy.KMinVy, phy.KMaxVy);

        phy.x += phy.vx;
        phy.y += phy.vy;
    }
    return true;
}
