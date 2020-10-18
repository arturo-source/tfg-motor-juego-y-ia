#pragma once
#include <ecs/util/typealiases.hpp>

namespace ECS {
struct Component_t  {
    explicit Component_t(EntityID_t eid) 
    : entityID {eid}
    {}
    
    constexpr EntityID_t getEntityID() const noexcept { return entityID; }

    
private:
    inline static ComponentID_t nextID { 0 };
    ComponentID_t ComponentID { ++nextID };
    EntityID_t entityID { 0 };
};

} // namespace ECS
