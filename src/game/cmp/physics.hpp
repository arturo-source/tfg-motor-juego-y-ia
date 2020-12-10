#pragma once
#include <ecs/cmp/component.hpp>

struct PhysicsComponent_t : public ECS::ComponentBase_t<PhysicsComponent_t> {
    explicit PhysicsComponent_t(ECS::EntityID_t eid) 
        : ComponentBase_t(eid)
    {}

    static constexpr float KMinVy    { -8 };
    static constexpr float KMaxVy    {  8 };
    static constexpr float KMinVx    { -4 };
    static constexpr float KMaxVx    {  4 };

    float friction {1};
    float x{0}, y{0};
    float vx{0}, vy{0};
    float aceleration { 0 };
};
