#pragma once
#include <ecs/util/typealiases.hpp>
#include <game/cmp/collider.hpp>

struct ColliderComponent_t;

template<typename GameCTX_t>
struct CollisionSystem_t {
    explicit CollisionSystem_t(uint32_t w, uint32_t h);

    bool update(GameCTX_t& g) const;
private:
    constexpr void checkBoundaryCollisions(const ColliderComponent_t& c, PhysicsComponent_t& p) const noexcept;
    constexpr BoundingBox_t move2ScreenCoords(const BoundingBox_t& box, float x, float y) const noexcept;
    constexpr bool checkObjectCollision(ColliderComponent_t& c1, ColliderComponent_t& c2, const PhysicsComponent_t& p1, const PhysicsComponent_t& p2) const noexcept;
    constexpr void react2Collision(GameCTX_t& g, ColliderComponent_t& c1, ColliderComponent_t& c2) const noexcept;
    constexpr void bounceBall(GameCTX_t& g, ColliderComponent_t& ball, ColliderComponent_t& player) const noexcept;
    
    const uint32_t m_w{0}, m_h{0};
};
