#pragma once
#include <cstdint>
#include <cmp/component.hpp>
#include <X11/X.h>
#include <X11/keysym.h>

namespace ECS {
struct InputComponent_t : public Component_t {
    explicit InputComponent_t(EntityID_t eid) 
        : Component_t(eid)
    {}

    static ComponentTypeID_t getComponentTypeID() noexcept {
        return static_cast<ComponentTypeID_t>(1);
    }

    KeySym key_LEFT  {XK_a};
    KeySym key_RIGHT {XK_d};
    KeySym key_UP    {XK_w};
    KeySym key_DOWN  {XK_s};
};
    
} // namespace ECS
