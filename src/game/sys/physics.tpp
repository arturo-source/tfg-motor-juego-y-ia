#include <game/sys/physics.hpp>
#include <game/cmp/physics.hpp>

template<typename GameCTX_t>
bool PysicsSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& phy: g.template getComponents<PhysicsComponent_t>()) {
        //Gravity
        phy.vy += phy.aceleration;
        //Keep velocity variable between min and max
        phy.vy = std::clamp(phy.vy, phy.KMinVy, phy.KMaxVy);

        phy.x += phy.vx;
        phy.y += phy.vy;
        dumpCSV(phy);
    }
    return true;
}

template<typename GameCTX_t>
void PysicsSystem_t<GameCTX_t>::dumpBin(const PhysicsComponent_t& phy) const {
    CurrentPhysics_t cp {
        phy.getEntityID(),
        phy.x, phy.y,
        phy.vx, phy.vy,
        phy.aceleration
    };
    std::ofstream file(filename.c_str(), std::ios::app | std::ios::binary);
    file.write(reinterpret_cast<const char*>(&cp), sizeof(cp));
    file.close();
}

template<typename GameCTX_t>
void PysicsSystem_t<GameCTX_t>::initCSV(std::string fname) {
    filename = fname;
    std::ofstream file(filename.c_str(), std::ios::app);
    file << "EntityID;x;y;vx;vy;aceleration\n";
    file.close();
}

template<typename GameCTX_t>
void PysicsSystem_t<GameCTX_t>::dumpCSV(const PhysicsComponent_t& phy) const {
    std::ofstream file(filename.c_str(), std::ios::app);
    file << phy.getEntityID() << ";" << phy.x << ";" << phy.y << ";" << phy.vx << ";" << phy.vy << ";" << phy.aceleration << "\n";
    file.close();
}