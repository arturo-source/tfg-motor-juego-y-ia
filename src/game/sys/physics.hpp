#pragma once
#include <ecs/util/typealiases.hpp>

struct CurrentPhysics_t {
    ECS::EntityID_t player_ID;
    uint32_t x, y;
    int32_t vx, vy;
    int32_t aceleration;
};

template<typename GameCTX_t>
struct PysicsSystem_t {
    explicit PysicsSystem_t() = default;

    std::string filename;

    bool update(GameCTX_t& g) const;
    void dump(const PhysicsComponent_t& phy) const;
private:    
};
