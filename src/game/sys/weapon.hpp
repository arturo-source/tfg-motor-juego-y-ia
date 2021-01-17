#pragma once

template<typename GameCTX_t>
struct WeaponSystem_t {
    explicit WeaponSystem_t() = default;

    void update(GameCTX_t& g) const;
};
