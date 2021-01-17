#pragma once
#include <cstdint>
#include <ecs/cmp/component.hpp>

struct WeaponComponent_t : public ECS::ComponentBase_t<WeaponComponent_t> {
    explicit WeaponComponent_t(ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {}

    constexpr void setCooldown() { shoot_cooldown = KMaxCooldown; }
    
    static constexpr uint32_t KMaxCooldown { 10 };

    uint32_t bullets { 5 };
    uint32_t shoot_cooldown { 0 };
};
