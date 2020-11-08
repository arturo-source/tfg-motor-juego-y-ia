#pragma once
#include <cstdint>
#include <ecs/cmp/component.hpp>

struct BoundingBox_t {
    uint32_t xLeft {0}, xRight {0};
    uint32_t yUp {0}, yDown {0};
    bool collided { false };
};


struct ColliderComponent_t : public ECS::ComponentBase_t<ColliderComponent_t> {
    explicit ColliderComponent_t(ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {}

    enum {
        L_NoLayer   = 0x00,
        L_Blade     = 0x01,
        L_Platforms = 0x02
    }

    uint8_t mask { 0xFF };
    BoundingBox_t box;
};
