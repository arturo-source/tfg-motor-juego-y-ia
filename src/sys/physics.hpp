#pragma once
#include <util/typealiases.hpp>

namespace ECS {

struct GameContext_t;

struct PysicsSystem_t {
    explicit PysicsSystem_t() = default;

    bool update(GameContext_t& g) const;
private:
};

} //namespace ECS
