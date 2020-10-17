#pragma once
#include <cstdint>
#include <cmp/component.hpp>

namespace ECS {
struct PhysicsComponent_t : public Component_t {
    explicit PhysicsComponent_t(EntityID_t eid) 
        : Component_t(eid)
    {}
    static ComponentTypeID_t getComponentTypeID() noexcept {
        return static_cast<ComponentTypeID_t>(2);
    }
    
    uint32_t x{0}, y{0};
    int32_t vx{1}, vy{1};
};
    
} // namespace ECS
