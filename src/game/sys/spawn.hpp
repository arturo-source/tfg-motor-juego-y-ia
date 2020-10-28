#pragma once
#include <ecs/util/typealiases.hpp>

struct SpawnerComponent_t;
struct PhysicsComponent_t;

template<typename GameCTX_t>
struct SpawnSystem_t {
    explicit SpawnSystem_t() = default;

    void update(GameCTX_t& g) const;
};
