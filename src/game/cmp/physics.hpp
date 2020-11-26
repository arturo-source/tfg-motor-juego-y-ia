#pragma once
#include <cstdint>
#include <ecs/cmp/component.hpp>

struct PhysicsComponent_t : public ECS::ComponentBase_t<PhysicsComponent_t> {
    explicit PhysicsComponent_t(ECS::EntityID_t eid) 
        : ComponentBase_t(eid)
    {}

    static constexpr int32_t KMultiplierY { 6 };
    static constexpr int32_t KGravity    { 1 };
    static constexpr int32_t KMinVy      { -10*KMultiplierY };
    static constexpr int32_t KMaxVy      { 10*KMultiplierY };
    static constexpr int32_t KMinVx      { -4 };
    static constexpr int32_t KMaxVx      { 4 };

    static constexpr std::array<int32_t, 7> jumpTable {
        -3, -3, -2, -2, -1, -1
    };

    void    setVy(int32_t newvy)   { vy = newvy*KMultiplierY; }
    int32_t getVy() const noexcept { return vy/KMultiplierY; }

    uint32_t x{0}, y{0};
    int32_t vx{0}, vy{0};
    int32_t gravity     { KGravity };
    int32_t aceleration { 0 };
    uint8_t jumpIdx     { jumpTable.size() };
};
