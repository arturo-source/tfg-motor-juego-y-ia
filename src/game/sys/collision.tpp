#include <game/sys/collision.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/collider.hpp>

template<typename GameCTX_t>
CollisionSystem_t<GameCTX_t>::CollisionSystem_t(uint32_t w, uint32_t h)
    : m_w(w), m_h(h)
    {}

template<typename GameCTX_t>
bool CollisionSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& col: g.template getComponents<ColliderComponent_t>()) {
        if(auto* phy = g.template getRequiredComponent<ColliderComponent_t, PhysicsComponent_t>(col)) { // phy != nullptr
            auto xl { phy->x + col.box.xLeft  };
            auto xr { phy->x + col.box.xRight };
            auto yu { phy->y + col.box.yUp    };
            auto yd { phy->y + col.box.yDown  };
            if(xl > m_w || xr > m_w) { phy->x -= phy->vx; phy->vx = -phy->vx; }
            if(yu > m_h || yd > m_h) { phy->y -= phy->vy; phy->vy = -phy->vy; }
        }
    }
    return true;
}
