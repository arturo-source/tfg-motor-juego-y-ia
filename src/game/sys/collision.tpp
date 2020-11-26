#include <game/sys/collision.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/collider.hpp>
#include <game/cmp/health.hpp>
#include <game/cmp/score.hpp>

template<typename GameCTX_t>
CollisionSystem_t<GameCTX_t>::CollisionSystem_t(uint32_t w, uint32_t h)
    : m_w(w), m_h(h)
    {}

template<typename GameCTX_t>
bool CollisionSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    auto& ColCmpVec {g.template getComponents<ColliderComponent_t>()};

    for(auto& c : ColCmpVec) c.box.collided = false;
    
    for(std::size_t i=0; i<ColCmpVec.size(); ++i) {
        auto& c1 { ColCmpVec[i] };
        auto* p1 = g.template getRequiredComponent<PhysicsComponent_t>(c1);
        if(!p1) continue; // p1 == nullptr
        checkBoundaryCollisions(c1, *p1);
        
        for(std::size_t j=i+1; j<ColCmpVec.size(); ++j) {
            auto& c2 { ColCmpVec[j] };
            if( (c1.mask & c2.mask) == 0) continue;
            auto* p2 = g.template getRequiredComponent<PhysicsComponent_t>(c2);
            if(!p2) continue; // p2 == nullptr
            
            if( checkObjectCollision(c1, c2, *p1, *p2)) 
                react2Collision(g, c1, c2);
        }
    }
    return true;
}

template <typename GameCTX_t>
constexpr void CollisionSystem_t<GameCTX_t>::inflictDamage(GameCTX_t& g, ColliderComponent_t& inflicter, ColliderComponent_t& receiver) const noexcept {
    auto* receivHealth = g.template getRequiredComponent<HealthComponent_t>(receiver);
    auto* infliHealth = g.template getRequiredComponent<HealthComponent_t>(inflicter);
    if(!receivHealth || !infliHealth) return;

    receivHealth->damage += infliHealth->damageInflicted;
    infliHealth->damage += infliHealth->selfDamageOnInfliction;
}

template <typename GameCTX_t>
constexpr void CollisionSystem_t<GameCTX_t>::undoCollision(GameCTX_t& g, ColliderComponent_t& solid, ColliderComponent_t& mobile) const noexcept {
    auto* solidPhy = g.template getRequiredComponent<PhysicsComponent_t>(solid);
    auto* mobilePhy = g.template getRequiredComponent<PhysicsComponent_t>(mobile);
    if(!solidPhy || !mobilePhy) return;
    
    //Translate to screen coords
    auto solidBox = move2ScreenCoords(solid.box, solidPhy->x, solidPhy->y);
    auto mobileBox = move2ScreenCoords(mobile.box, mobilePhy->x, mobilePhy->y);

    auto intervalIntersection = [](uint32_t Ml, uint32_t Mr, uint32_t Sl, uint32_t Sr) -> int32_t {
        if(Ml < Sl) {
            if(Mr < Sr)    return Sl - Mr;
        } else if(Mr > Sr) return Sr - Ml;
        return 0;
    };

    //Calculate intersection
    struct { int32_t x,y; } overlap {
        intervalIntersection(mobileBox.xLeft, mobileBox.xRight, solidBox.xLeft, solidBox.xRight)
        ,intervalIntersection(mobileBox.yUp, mobileBox.yDown, solidBox.yUp, solidBox.yDown)
    };

    //Undo overlap
    if ((overlap.x == 0) || ( overlap.y != 0 && std::abs(overlap.y) <= std::abs(overlap.x))) {
        mobilePhy->y += overlap.y;
        mobilePhy->vy = 0;
    } else {
        mobilePhy->x += overlap.x;
        mobilePhy->vx = 0;
    }
}

template <typename GameCTX_t>
constexpr void CollisionSystem_t<GameCTX_t>::bounceBall(GameCTX_t& g, ColliderComponent_t& ball, ColliderComponent_t& player) const noexcept {
    auto* ballPhy = g.template getRequiredComponent<PhysicsComponent_t>(ball);
    auto* playerPhy = g.template getRequiredComponent<PhysicsComponent_t>(player);
    if(!ballPhy || !playerPhy) return;

    ballPhy->vx = -ballPhy->vx;
    ballPhy->vy -= playerPhy->vy;
}

template <typename GameCTX_t>
constexpr void CollisionSystem_t<GameCTX_t>::react2Collision(GameCTX_t& g, ColliderComponent_t& c1, ColliderComponent_t& c2) const noexcept {
    using CP = ColliderComponent_t;
    CP *player { &c1 }, *other { &c2 };

    if(c2.properties & CP::P_IsPlayer) { std::swap(player, other); }
    else if(!(c1.properties & CP::P_IsPlayer)) return;

    if(other->properties & CP::P_IsBall) {
        bounceBall(g, *other, *player);
    }

    // if(other->properties & CP::P_Damages) {
    //     inflictDamage(g, *other, *player);
    // } else if(other->properties & CP::P_IsSolid) {
    //     undoCollision(g, *other, *player);
    // }
}

template <typename GameCTX_t>
constexpr bool CollisionSystem_t<GameCTX_t>::checkObjectCollision(ColliderComponent_t& c1, ColliderComponent_t& c2, const PhysicsComponent_t& p1, const PhysicsComponent_t& p2) const noexcept {
    // Move Bounding Boxes to screen coordinates
    auto b1 { move2ScreenCoords(c1.box, p1.x, p1.y) };
    auto b2 { move2ScreenCoords(c2.box, p2.x, p2.y) };

    auto checkIntervals = [](uint32_t L1, uint32_t R1, uint32_t L2, uint32_t R2) {
        if(L2 > R1) return false;
        if(L1 > R2) return false;
        return true;
    };

    // Check collision in both axes
    if( checkIntervals(b1.xLeft, b1.xRight, b2.xLeft, b2.xRight) 
     && checkIntervals(b1.yUp, b1.yDown, b2.yUp, b2.yDown)) {
        c1.box.collided = true;
        c2.box.collided = true;
        return true;
    }
    return false;
}

template <typename GameCTX_t>
constexpr BoundingBox_t CollisionSystem_t<GameCTX_t>::move2ScreenCoords(const BoundingBox_t& box, uint32_t x, uint32_t y) const noexcept {
    BoundingBox_t screenBox {
        x + box.xLeft
    ,   x + box.xRight
    ,   y + box.yUp
    ,   y + box.yDown
    };
    return screenBox;
}

template <typename GameCTX_t>
constexpr void CollisionSystem_t<GameCTX_t>::checkBoundaryCollisions(const ColliderComponent_t& c, PhysicsComponent_t& p) const noexcept {
    auto b {move2ScreenCoords(c.box, p.x, p.y)};
    if(b.xLeft > m_w || b.xRight > m_w) { 
        p.x -= p.vx;
        if(c.properties & ColliderComponent_t::P_IsBall) {
            p.vx = 0;
            ScoreComponent_t::scorePosX = p.x;
            ScoreComponent_t::scored = true;
        }
        // p.x -= p.vx; p.vx = -p.vx;
    }
    if(b.yUp > m_h || b.yDown > m_h) { 
        p.y -= p.getVy();
        if(c.properties & ColliderComponent_t::P_IsBall) {
            p.vy = -p.vy;
        } else if(c.properties & ColliderComponent_t::P_IsPlayer) {
            p.vy = 0;
        }
    }
}
