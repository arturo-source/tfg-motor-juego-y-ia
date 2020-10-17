#pragma once
#include <cstdint>
#include <cmp/physics.hpp>
#include <cmp/render.hpp>
#include <cmp/input.hpp>
#include <util/typealiases.hpp>

namespace ECS {

struct Entity_t {
    explicit Entity_t() = default;

    template<typename CMP_t>
    void addComponent(CMP_t& cmp) {
        auto type = cmp.getComponentTypeID();
        m_components[type] = &cmp;
    }

    template<typename CMP_t>
    CMP_t* getComponent() {
        auto type = CMP_t::getComponentTypeID();
        auto it = m_components.find(type);
        if(it != m_components.end()) 
            return static_cast<CMP_t*>(it->second);
        return nullptr;
    }

    template<typename CMP_t>
    const CMP_t* getComponent() const {
        auto* cmp = const_cast<Entity_t*>(this)->getComponent<CMP_t>();
        return const_cast<const CMP_t*>(cmp);
    }

    constexpr EntityID_t getEntityID() const noexcept { return entityID; }
private:
    Hash_t<ComponentTypeID_t, Component_t*> m_components;

    EntityID_t entityID { ++nextID };
    inline static EntityID_t nextID {0};
};
} // namespace ECS
