#include <game/util/gameobjectfactory.hpp>

ECS::Entity_t& GameObjectFactory_t::createEntity(uint32_t x, uint32_t y, const std::string_view filename) const {
    auto& e = m_EntMan.createEntity(); // leer filename¡¡
    auto& rn = m_EntMan.addComponent<RenderComponent_t>(e);
    auto& ph = m_EntMan.addComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntMan.addComponent<ColliderComponent_t>(e);
    m_EntMan.addComponent<HealthComponent_t>(e);
    rn.loadFromFile(filename);
    ph.x = x; ph.y = y;
    ph.vx = 1; ph.vy = 1;
    cl.box.xLeft  = 0;
    cl.box.xRight = rn.w;
    cl.box.yUp    = 0;
    cl.box.yDown  = rn.h;

    return e;
}

ECS::Entity_t& GameObjectFactory_t::createPlayer(uint32_t x, uint32_t y) const {
    auto& e = createEntity(x, y, "assets/character.png");
    m_EntMan.addComponent<InputComponent_t>(e);
    
    auto* h = e.getComponent<HealthComponent_t>();
    if(!h) return e;
    h->health = 100;
    
    auto* col = e.getComponent<ColliderComponent_t>();
    if(!col) return e; 
    col->properties = ColliderComponent_t::P_IsPlayer; 

    return e;
}

ECS::Entity_t& GameObjectFactory_t::createEnemy(uint32_t x, uint32_t y) const {
    auto& e = createEntity(x, y, "assets/enemy.png");
    
    auto* col = e.getComponent<ColliderComponent_t>();
    if(col) { 
        col->mask       = ColliderComponent_t::L_Enemy; 
        col->properties = ColliderComponent_t::P_Damages; 
    }
    
    auto* phy = e.getComponent<PhysicsComponent_t>();
    if(phy) { phy->gravity = 0; }

    auto* hea = e.getComponent<HealthComponent_t>();
    if(hea) { hea->damageInflicted = 1; hea->selfDamageOnInfliction = 1; }

    return e;
}

ECS::Entity_t& GameObjectFactory_t::createEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) const {
    auto& e = m_EntMan.createEntity();
    auto& rn = m_EntMan.addComponent<RenderComponent_t>(e);
    auto& ph = m_EntMan.addComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntMan.addComponent<ColliderComponent_t>(e);
    rn.loadFromWH(w, h, color);
    ph.x  = x; ph.y  = y;
    ph.vx = 0; ph.vy = 0;
    ph.gravity = 0;
    cl.box.xLeft  = 0;
    cl.box.xRight = rn.w;
    cl.box.yUp    = 0;
    cl.box.yDown  = rn.h;

    return e;
}

ECS::Entity_t& GameObjectFactory_t::createPalette(uint32_t x, uint32_t y, uint8_t side) const {
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
ECS::Entity_t& GameObjectFactory_t::createBall(uint32_t x, uint32_t y) const {
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