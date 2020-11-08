#pragma once
#include <ecs/util/typealiases.hpp>
#include <game/cmp/collider.hpp> //para tener el BoundingBox

template<typename GameCTX_t>
struct HealthSystem_t {
    explicit HealthSystem_t() = default;

    constexpr void update(GameCTX_t& g) const noexcept;
private:
    constexpr bool leafNodeCollided(const BoundingBox_t& bb) const noexcept;
};