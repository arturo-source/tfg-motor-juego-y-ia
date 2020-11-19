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