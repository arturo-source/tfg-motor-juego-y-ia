#pragma once
#include <cstdint>
#include <ecs/cmp/component.hpp>

struct BoundingBox_t {
    float xLeft {0}, xRight {0};
    float yUp {0}, yDown {0};
    bool collided { false };
};


struct ColliderComponent_t : public ECS::ComponentBase_t<ColliderComponent_t> {
    explicit ColliderComponent_t(ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {}

    enum {
        P_Nothing  = 0x00,
        P_IsPlayer = 0x01,
        P_Bounces  = 0x02,
        P_IsBall   = 0x04,
        P_IsBullet = 0x08,
        P_IsWall   = 0x10
    };

    uint8_t properties { P_Nothing };
    BoundingBox_t box;
};
