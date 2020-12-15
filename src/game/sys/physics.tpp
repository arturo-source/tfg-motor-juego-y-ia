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
        if(inp) AI_system.dumpCSV(phy, inp->side);
        else    AI_system.dumpCSV(phy, InputComponent_t::S_NoSide); //Ball doesnt have input
    }
    return true;
}

template<typename GameCTX_t>
void PysicsSystem_t<GameCTX_t>::initCSV(std::string fname) {
    AI_system.physicsfilename = fname;
    std::ofstream file(fname.c_str(), std::ios::app);
    if(!file) throw std::runtime_error("Can't open physics CSV file for write\n");

    file << "Side;x;y;vx;vy;aceleration\n";
    file.close();
}