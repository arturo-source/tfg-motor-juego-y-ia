#include <game/sys/weapon.hpp>
#include <game/cmp/weapon.hpp>

template<typename GameCTX_t>
void WeaponSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& weap: g.template getComponents<WeaponComponent_t>()) {
        if(weap.shoot_cooldown > 0) --weap.shoot_cooldown;
    }
}