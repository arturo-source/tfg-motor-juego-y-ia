#pragma once
#include <ecs/util/keys.hpp>
#include <cstdint>
#include <ecs/cmp/component.hpp>

struct InputComponent_t : public ECS::ComponentBase_t<InputComponent_t> {
    explicit InputComponent_t(ECS::EntityID_t eid) 
        : ComponentBase_t(eid)
    {}

    enum {
        S_NoSide = 0x00,
        S_Left   = 0x01,
        S_Right  = 0x02,
        S_Center = 0x04
    };
    uint8_t side { S_NoSide };

    ECS::Key_t key_UP    {ECS::w};
    ECS::Key_t key_DOWN  {ECS::s};
    ECS::Key_t key_shoot {ECS::Tab};
};
