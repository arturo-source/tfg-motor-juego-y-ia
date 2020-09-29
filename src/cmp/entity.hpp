#pragma once
#include <cstdint>
#include <cmp/physics.hpp>
#include <cmp/render.hpp>
#include <util/typealiases.hpp>

namespace ECS {

struct Entity_t {
    explicit Entity_t() = default;
    
    PhysicsComponent_t* phy {nullptr};
    RenderComponent_t* ren {nullptr};
    EntityID_t entityID { ++nextID };

private:
    inline static EntityID_t nextID {0};
};
} // namespace ECS
