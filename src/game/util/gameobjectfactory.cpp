#include <game/util/gameobjectfactory.hpp>

ECS::Entity_t& GameObjectFactory_t::createEntity(float x, float y, uint32_t w, uint32_t h, uint32_t color) const {
    auto& e = m_EntMan.createEntity();
    auto& rn = m_EntMan.addComponent<RenderComponent_t>(e);
    auto& ph = m_EntMan.addComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntMan.addComponent<ColliderComponent_t>(e);
    rn.loadFromWH(w, h, color);
    ph.x  = x; ph.y  = y;
    ph.vx = 0; ph.vy = 0;
    cl.box.xLeft  = 0;
    cl.box.xRight = rn.w;
    cl.box.yUp    = 0;
    cl.box.yDown  = rn.h;

    return e;
}

ECS::Entity_t& GameObjectFactory_t::createPalette(float x, float y, uint8_t side) const {
    constexpr uint32_t w { 10 }, h { 100 };
    auto& e = createEntity(x - w/2 , y - h/2, w, h, 0xFFFF0000);
    auto& sco = m_EntMan.addComponent<ScoreComponent_t>(e);
    auto& inp = m_EntMan.addComponent<InputComponent_t>(e);
    if(side & InputComponent_t::S_Right) {
        inp.key_UP   = XK_o;
        inp.key_DOWN = XK_l;
    }
    inp.side = side;
    auto* col = e.getComponent<ColliderComponent_t>();
    if(col) {
        col->properties = ColliderComponent_t::P_IsPlayer;
    }
    
    return e;
}
ECS::Entity_t& GameObjectFactory_t::createBall(float x, float y) const {
    constexpr uint32_t w { 10 }, h { 10 };
    auto& e = createEntity(x - w/2 , y - h/2, w, h, 0xFF0000FF);
    m_EntMan.addComponent<HealthComponent_t>(e);

    auto* phy = e.getComponent<PhysicsComponent_t>();
    if(phy) {
        phy->vx = 3;
    }
    auto* col = e.getComponent<ColliderComponent_t>();
    if(col) {
        col->properties = ColliderComponent_t::P_IsBall;
    }

    return e;
}