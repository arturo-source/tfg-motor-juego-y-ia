#pragma once
#include <ecs/util/typealiases.hpp>
#include <game/man/game.hpp>

template<typename GameCTX_t>
struct ScoreboardSystem_t {
    explicit ScoreboardSystem_t(const uint32_t w) : avgwidth {w/2} {}

    void update(GameCTX_t& g) const;
    
    inline static float scorePosX { 0 };
    inline static bool  scored    { false };
    inline static bool  end_game  { false };
private:
    const uint32_t avgwidth { 0 };
};
