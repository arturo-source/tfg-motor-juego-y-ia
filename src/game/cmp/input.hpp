#pragma once
#include <cstdint>
#include <ecs/cmp/component.hpp>
#include <X11/X.h>
#include <X11/keysym.h>

struct InputComponent_t : public ECS::ComponentBase_t<InputComponent_t> {
    explicit InputComponent_t(ECS::EntityID_t eid) 
        : ComponentBase_t(eid)
    {}

    // KeySym key_LEFT  {XK_a};
    // KeySym key_RIGHT {XK_d};
    KeySym key_UP    {XK_w};
    KeySym key_DOWN  {XK_s};
};
