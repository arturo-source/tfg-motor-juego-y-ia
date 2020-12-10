#pragma once
#include <ecs/cmp/component.hpp>

struct InteligenceComponent_t : public ECS::ComponentBase_t<InteligenceComponent_t> {
    explicit InteligenceComponent_t(ECS::EntityID_t eid) 
        : ComponentBase_t(eid)
    {}
    
};