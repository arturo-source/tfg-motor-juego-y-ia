#pragma once
#include <ecs/util/typealiases.hpp>
#include <game/sys/artificialinteligence.hpp>

template<typename GameCTX_t>
struct PysicsSystem_t {
    explicit PysicsSystem_t() = default;

    bool update(GameCTX_t& g) const;
};
