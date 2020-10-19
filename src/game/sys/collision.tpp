#include <game/sys/collision.hpp>

template<typename GameCTX_t>
bool CollisionSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& e: g.getEntities()) {
        auto phy = e.template getComponent<PhysicsComponent_t>();
        auto ren = e.template getComponent<RenderComponent_t>();
        if(phy && ren) { // phy != nullptr && ren != nullptr
            if(phy->x > 640 || phy->x + ren->w > 640) {phy->x -= phy->vx; phy->vx = -phy->vx;}
            if(phy->y > 360 || phy->y + ren->h > 360) {phy->y -= phy->vy; phy->vy = -phy->vy;}
        }
    }
    return true;
}
