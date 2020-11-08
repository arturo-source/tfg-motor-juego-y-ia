#pragma once
#include <algorithm>
#include <ecs/man/entitymanager.hpp>

namespace ECS {

    inline EntityManager_t::EntityManager_t() {
        m_Entity.reserve(kNUMINITIALENTITIES);
    }

    inline Entity_t& EntityManager_t::createEntity() {
        return m_Entity.emplace_back();
    }

    inline void EntityManager_t::destroyEntityByID(EntityID_t eid) {
        auto* entity { getEntityByID(eid) };
        if(!entity) return;

        for(auto& [typeID, cmp]: *entity) {
            auto* cmpptr = m_components.deleteComponentByTypeIDAndEntityID(typeID, eid);
            if(!cmpptr) continue;
            auto* movedEntity { getEntityByID(cmpptr->getEntityID()) };
            movedEntity->updateComponent(typeID, cmpptr);            
        }

        //COPY PASTE MAL!!!
        auto it = std::find_if(m_Entity.begin(), m_Entity.end(), 
            [&](const Entity_t& e){ return eid == e.getEntityID(); }
        );
        m_Entity.erase(it);
    }

    inline const Entity_t* EntityManager_t::getEntityByID(EntityID_t eid) const {
        auto it = std::find_if(m_Entity.begin(), m_Entity.end(), 
            [&](const Entity_t& e){ return eid == e.getEntityID(); }
        );
        if(it == m_Entity.end())
            return nullptr;

        return it.base();
    }
    inline Entity_t* EntityManager_t::getEntityByID(EntityID_t eid) {
        auto eptr = const_cast<const EntityManager_t*>(this)->getEntityByID(eid);
        return const_cast<Entity_t*>(eptr);
    }

    template<typename CMP_t>
    CMP_t& EntityManager_t::addComponent(Entity_t& e) {
        CMP_t* cmp_ptr { e.getComponent<CMP_t>() };
        if( !cmp_ptr ) { //cmp_ptr == nullptr
                auto& cmp = m_components.createComponent<CMP_t>(e.getEntityID());
                e.addComponent(cmp);
                cmp_ptr = &cmp;
        }
        return *cmp_ptr;
    }

    template<typename CMP_t>
    Entity_t* EntityManager_t::getEntityPointerFromComponent(const CMP_t& cmp) {
        return getEntityByID( cmp.getEntityID() );
    }
    template<typename CMP_t>
    const Entity_t* EntityManager_t::getEntityPointerFromComponent(const CMP_t& cmp) const {
        return getEntityByID( cmp.getEntityID() );
    }

    template<typename ReqCMP_t, typename CMP_t>
    const ReqCMP_t* EntityManager_t::getRequiredComponent(const CMP_t& cmp) const {
        auto* e = getEntityPointerFromComponent(cmp);
        if(e)
                return e->template getComponent<ReqCMP_t>();
        return nullptr;
    }
    template<typename ReqCMP_t, typename CMP_t>
    ReqCMP_t* EntityManager_t::getRequiredComponent(const CMP_t& cmp) {
        const ReqCMP_t* rc = const_cast<const EntityManager_t*>(this)->getRequiredComponent<ReqCMP_t, CMP_t>(cmp);
        return const_cast<ReqCMP_t*>(rc);
    }
} // namespace ECS
