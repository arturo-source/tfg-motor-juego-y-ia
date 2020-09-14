#pragma once
#include <util/typealiases.hpp>

namespace ECS {
struct Component_t  {
    explicit Component_t(EntityID_t eid) 
    : entityID {eid}
    {}

    ComponentID_t ComponentID { ++nextID };
    EntityID_t entityID { 0 };
private:
    inline static ComponentID_t nextID { 0 };
};

} // namespace ECS
