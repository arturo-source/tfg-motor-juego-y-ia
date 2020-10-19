#pragma once
#include <cstdint>
#include <ecs/cmp/component.hpp>
#include <X11/X.h>
#include <X11/keysym.h>

struct InputComponent_t : public ECS::Component_t {
    explicit InputComponent_t(ECS::EntityID_t eid) 
        : Component_t(eid)
    {}

    static ECS::ComponentTypeID_t getComponentTypeID() noexcept {
        return static_cast<ECS::ComponentTypeID_t>(1);
    }

    KeySym key_LEFT  {XK_a};
    KeySym key_RIGHT {XK_d};
    KeySym key_UP    {XK_w};
    KeySym key_DOWN  {XK_s};
};
