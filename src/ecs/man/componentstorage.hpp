#pragma once
#include <vector>
#include <algorithm>
#include <ecs/util/typealiases.hpp>
#include <ecs/man/componentvector.hpp>

namespace ECS {

struct ComponentStorage_t {
    ComponentStorage_t() = delete;
    explicit ComponentStorage_t(std::size_t initialsize) 
    : m_initialSize(initialsize)
    {}
    ComponentStorage_t(const ComponentStorage_t&) = delete;
    ComponentStorage_t(ComponentStorage_t&&) = delete;
    ComponentStorage_t operator=(const ComponentStorage_t&) = delete;
    ComponentStorage_t operator=(ComponentStorage_t&&) = delete;
    
    template<typename CMP_t>       CMP_t&        createComponent(EntityID_t eid);
    template<typename CMP_t>       Vec_t<CMP_t>& createComponentVector();
    template<typename CMP_t>       Vec_t<CMP_t>& getComponents();
    template<typename CMP_t> const Vec_t<CMP_t>& getComponents() const;

    Component_t* deleteComponentByTypeIDAndEntityID(ComponentTypeID_t cid, EntityID_t eid);
private:
    Hash_t<ComponentTypeID_t, UPtr<ComponentVectorBase_t>> m_componentVectors;
    std::size_t m_initialSize {100};
};

} // namespace ECS
