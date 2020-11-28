#include <game/sys/scoreboard.hpp>

template<typename GameCTX_t>
void ScoreboardSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& sco: g.template getComponents<ScoreComponent_t>()) {
        if(ScoreComponent_t::scored) {
            auto* phy = g.template getRequiredComponent<PhysicsComponent_t>(sco);
            if(!phy) continue;
            if((ScoreComponent_t::scorePosX < avgwidth && phy->x > avgwidth) ||
              (ScoreComponent_t::scorePosX > avgwidth && phy->x < avgwidth) ) {
                sco.score++;
            }
        }
    }
}

template<typename GameCTX_t>
constexpr void ScoreboardSystem_t<GameCTX_t>::show(GameCTX_t& g) const { 
    std::cout << "|";
    for(auto& sco: g.template getComponents<ScoreComponent_t>()) {
        std::cout << " Player" << sco.getEntityID() << ": " << sco.score << " |";
    }
    std::cout << "\n";
}
