#include <game/sys/health.hpp>
#include <game/cmp/health.hpp>

template<typename GameCTX_t>
constexpr bool HealthSystem_t<GameCTX_t>::leafNodeCollided(const BoundingBox_t& bb) const noexcept {
    return bb.collided;
}

template<typename GameCTX_t>
constexpr void HealthSystem_t<GameCTX_t>::update(GameCTX_t& g) const noexcept {
    for(auto& h: g.template getComponents<HealthComponent_t>()) {
        const auto* c { g.template getRequiredComponent<ColliderComponent_t>(h)};
        if(!c) continue;
        if( h.health && leafNodeCollided(c->box) ) {
            if( --h.health == 0) {
                std::cout << "Ha muerto " << h.getEntityID() << "!\n";
                g.destroyEntityByID(h.getEntityID());
            }
        }
    }
}
