#pragma once
#include <ecs/util/typealiases.hpp>
#include <ecs/cmp/component.hpp>
#include <algorithm>
#include <optional>

namespace ECS {
struct ComponentVectorBase_t {
    virtual ~ComponentVectorBase_t() = default;
    virtual Component_t* deleteComponentByEntityID(EntityID_t eid) = 0;
};

template<typename CMP_t>
struct ComponentVector_t : ComponentVectorBase_t{
    Vec_t<CMP_t> components;

    constexpr auto findComponentIteratorByID(EntityID_t eid) noexcept {
        //TODO: Change!! (linear search)
        std::optional itopt =
        std::find_if(components.begin(), components.end(), 
            [&eid](CMP_t& cmp) { return cmp.getEntityID() == eid; }
        );
        if(itopt == components.end() ) itopt.reset();

        return itopt;
    }

    Component_t* deleteComponentByEntityID(EntityID_t eid) override {
        auto itopt = findComponentIteratorByID(eid);
        if(!itopt) return nullptr;
        
        auto it = *itopt;
        if( it+1 != components.end())
            *it = components.back();
        components.pop_back();

        return it.base();
    }
};
} // namespace ECS
