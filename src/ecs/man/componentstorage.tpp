#pragma once
#include <ecs/man/componentstorage.hpp>

namespace ECS {
    template<typename CMP_t>
    CMP_t& ComponentStorage_t::createComponent(EntityID_t eid) {
        auto& v = getComponents<CMP_t>();
        auto& cmp = v.emplace_back(eid);
        return cmp;
    }

    template<typename CMP_t>
    Vec_t<CMP_t>& ComponentStorage_t::createComponentVector() {
        auto typeID = CMP_t::getComponentTypeID();
        auto v = std::make_unique<ComponentVector_t<CMP_t>>();
        v->components.reserve(m_initialSize);
        auto* vecptr = v.get();
        m_componentVectors[typeID] = std::move(v);
        return vecptr->components;
    }
    template<typename CMP_t>
    Vec_t<CMP_t>& ComponentStorage_t::getComponents() {
        Vec_t<CMP_t>* comvec { nullptr };
        auto typeID = CMP_t::getComponentTypeID();
        auto it = m_componentVectors.find(typeID);
        if(it != m_componentVectors.end()) {
            //HAY QUE COMPROBAR SI LO QUE DEVUELVE DYN_CAST DYNAMIC NO ES NULL
            auto* v = dynamic_cast<ComponentVector_t<CMP_t>*>(it->second.get());
            comvec = &(v->components);
        } else {
            comvec = &createComponentVector<CMP_t>();
        }
        return *comvec;
    }
    template<typename CMP_t>
    const Vec_t<CMP_t>& ComponentStorage_t::getComponents() const {
        Vec_t<CMP_t>* comvec { nullptr };
        auto typeID = CMP_t::getComponentTypeID();
        auto it = m_componentVectors.find(typeID);
        if(it != m_componentVectors.end()) {
            //HAY QUE COMPROBAR SI LO QUE DEVUELVE DYN_CAST DYNAMIC NO ES NULL
            auto* v = dynamic_cast<ComponentVector_t<CMP_t>*>(it->second.get());
            comvec = &(v->components);
        } else {
            // comvec = &createComponentVector();
            throw "Asi no hombre!!";
        }
        return *comvec;
    }

    inline Component_t* ComponentStorage_t::deleteComponentByTypeIDAndEntityID(ComponentTypeID_t cid, EntityID_t eid) {
        auto it = m_componentVectors.find(cid);
        if( it == m_componentVectors.end() ) return nullptr;
        auto vecBase { it->second.get() };
        return vecBase->deleteComponentByEntityID(eid);
    }

} // namespace ECS
