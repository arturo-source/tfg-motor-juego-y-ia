#include <game/sys/scoreboard.hpp>

template<typename GameCTX_t>
void ScoreboardSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& sco: g.template getComponents<ScoreComponent_t>()) {
        if(scored) {
            auto* phy = g.template getRequiredComponent<PhysicsComponent_t>(sco);
            if(!phy) continue;
            if((scorePosX < avgwidth && phy->x > avgwidth) ||
               (scorePosX > avgwidth && phy->x < avgwidth)) {
                sco.score++;
            }
            if(sco.score > 4) end_game = true;
        }
    }
}