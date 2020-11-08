#pragma once
#include <cstdint>
#include <ecs/util/typealiases.hpp>
#include <ecs/cmp/component.hpp>

namespace ECS {

struct Entity_t {
    explicit Entity_t() = default;

    template<typename CMP_t>
    void addComponent(CMP_t& cmp) {
        auto type = cmp.getComponentTypeID();
        m_components[type] = &cmp;
    }

    template<typename CMP_t>
    const CMP_t* getComponent() const {
        auto type = CMP_t::getComponentTypeID();
        auto it = m_components.find(type);
        if(it != m_components.end()) 
            return static_cast<CMP_t*>(it->second);
        return nullptr;
    }

    //Modificado porque el pr.retroman explica en la clase 62
    //que hacer un const_cast desde una funcion no const en una const
    //es peligroso porque puede que la no const modifique 
    template<typename CMP_t>
    CMP_t* getComponent() {
        const CMP_t* cmp = const_cast<const Entity_t*>(this)->getComponent<CMP_t>();
        return const_cast<CMP_t*>(cmp);
    }

    void updateComponent(ComponentTypeID_t cid, Component_t* cmpptr) {
        auto it = m_components.find(cid);
        if(it != m_components.end()) 
            it->second = cmpptr;
    }

    constexpr EntityID_t getEntityID() const noexcept { return entityID; }
    
    auto begin() const { return m_components.begin(); }
    auto end() const { return m_components.end(); }
private:
    Hash_t<ComponentTypeID_t, Component_t*> m_components;

    EntityID_t entityID { ++nextID };
    inline static EntityID_t nextID {0};
};
} // namespace ECS
