#pragma once
#include <ecs/util/typealiases.hpp>

template<typename GameCTX_t>
struct ScoreboardSystem_t {
    explicit ScoreboardSystem_t(uint32_t w) : avgwidth {w/2}
    {}

    void update(GameCTX_t& g) const;
private:
    const uint32_t avgwidth { 0 };
};
