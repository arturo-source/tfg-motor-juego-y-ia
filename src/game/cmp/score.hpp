#pragma once
#include <cstdint>
#include <ecs/cmp/component.hpp>

struct ScoreComponent_t : public ECS::ComponentBase_t<ScoreComponent_t> {
    explicit ScoreComponent_t(ECS::EntityID_t eid) 
        : ComponentBase_t(eid)
    {}
    uint32_t score { 0 };
    inline static float scorePosX { 0 };
    inline static bool  scored    { false };
};
