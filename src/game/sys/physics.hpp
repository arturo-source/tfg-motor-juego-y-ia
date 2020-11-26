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

    static constexpr std::size_t KMaxPhysicsStored = 1000;
    std::string filename;

    bool update(GameCTX_t& g) const;
    void addPhysic(const PhysicsComponent_t& phy) const;
    void dump() const;

private:    
    //PROBABLEMENTE ESTÁ MAL DECIDIR HACERLOS MUTABLES
    mutable uint32_t pphysics { 0 };
    mutable std::vector<CurrentPhysics_t> physics { KMaxPhysicsStored };    
};
