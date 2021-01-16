#pragma once
extern "C" {
    #ifdef windows
    #include <tinyPTC/src/windows/tinyptc.h>
    #else
    #include <tinyPTC/src/linux/tinyptc.h>
    #endif
}
#include <cstdint>
#include <ecs/cmp/component.hpp>

struct InputComponent_t : public ECS::ComponentBase_t<InputComponent_t> {
    explicit InputComponent_t(ECS::EntityID_t eid) 
        : ComponentBase_t(eid)
    {}

    enum {
        S_NoSide = 0x00,
        S_Left   = 0x01,
        S_Right  = 0x02
    };
    uint8_t side { S_NoSide };

    #ifdef windows
    KeySym key_UP    {'W'};
    KeySym key_DOWN  {'S'};
    #else
    KeySym key_UP    {XK_w};
    KeySym key_DOWN  {XK_s};
    #endif
};
