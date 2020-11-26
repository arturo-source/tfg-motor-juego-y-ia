#include <game/sys/physics.hpp>
#include <game/cmp/physics.hpp>

template<typename GameCTX_t>
bool PysicsSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& phy: g.template getComponents<PhysicsComponent_t>()) {
        //Jump
        // if( phy.jumpIdx < phy.jumpTable.size() ) {
        //     phy.vy += phy.jumpTable[phy.jumpIdx]; 
        //     ++phy.jumpIdx;
        // }

        //Gravity
        // phy.vy += phy.gravity;
        //Mantener variable entre un máx y un mín
        // phy.vy = std::clamp(phy.vy, phy.KMinVy, phy.KMaxVy);

        //Gravity
        phy.vy += phy.aceleration;
        //Mantener variable entre un máx y un mín
        phy.vy = std::clamp(phy.vy, phy.KMinVy, phy.KMaxVy);

        phy.x += phy.vx;
        phy.y += phy.getVy();
        addPhysic(phy);
    }
    return true;
}

template<typename GameCTX_t>
void PysicsSystem_t<GameCTX_t>::dump() const {
    std::ofstream file(filename.c_str(), std::ios::app | std::ios::binary);
    for(CurrentPhysics_t& phy: physics)
        if(phy.player_ID != 0)
            file.write(reinterpret_cast<const char*>(&phy), sizeof(phy));

    file.close();
    physics.clear();
    physics.resize(KMaxPhysicsStored);
}

template<typename GameCTX_t>
void PysicsSystem_t<GameCTX_t>::addPhysic(const PhysicsComponent_t& phy) const {
    CurrentPhysics_t cp {
        phy.getEntityID(),
        phy.x, phy.y,
        phy.vx, phy.getVy(),
        phy.aceleration
    };
    if(pphysics < physics.capacity()) {
        physics[pphysics] = cp;
        pphysics++;
    } else {
        dump();
        physics[0] = cp;
        pphysics = 1;
    }
}