#include <algorithm>
// #include <string_view>
#include <ecs/man/entitymanager.hpp>

namespace ECS {

    EntityManager_t::EntityManager_t() {
        m_Entity.reserve(kNUMINITIALENTITIES);
    }

    const Entity_t* EntityManager_t::getEntityByID(EntityID_t eid) const {
        auto it = std::find_if(m_Entity.begin(), m_Entity.end(), 
            [&](const Entity_t& e){ return eid == e.getEntityID(); }
        );
        if(it == m_Entity.end())
            return nullptr;

        return it.base();
    }
    Entity_t* EntityManager_t::getEntityByID(EntityID_t eid) {
        auto eptr = const_cast<const EntityManager_t*>(this)->getEntityByID(eid);
        return const_cast<Entity_t*>(eptr);
    }
    
    // Entity_t& EntityManager_t::createEntity(uint32_t x, uint32_t y, const std::string_view filename) {
    //     auto& e = m_Entity.emplace_back(); // leer filename¡¡
    //     auto eid = e.getEntityID(); 
    //     auto& ph = m_components.createComponent<PhysicsComponent_t>(eid);
    //     auto& rn = m_components.createComponent<RenderComponent_t>(eid);
    //     rn.loadFromFile(filename);
    //     e.addComponent(ph);
    //     e.addComponent(rn);
    //     ph.x = x; ph.y = y;
    //     // std::fill(begin(e.sprite), end(e.sprite), color);

    //     return e;
    // }

    // void EntityManager_t::addInputComponent(Entity_t& e) {
    //     if( !e.getComponent<InputComponent_t>() ){
    //         auto& cmp = m_components.createComponent<InputComponent_t>(e.getEntityID());
    //         e.addComponent(cmp);
    //     }
    // }
    
} // namespace ECS
