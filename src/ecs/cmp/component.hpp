#pragma once
#include <ecs/util/typealiases.hpp>

namespace ECS {
struct Component_t  {
    explicit Component_t(EntityID_t eid) 
    : entityID {eid}
    {}
    
    constexpr EntityID_t getEntityID() const noexcept { return entityID; }

protected:
    inline static ComponentTypeID_t nextTypeID { 0 };
private:
    // inline static ComponentID_t nextID { 0 };
    // ComponentID_t ComponentID { ++nextID };
    EntityID_t entityID { 0 };
};

template<typename TYPE>
struct ComponentBase_t : Component_t {
    explicit ComponentBase_t(EntityID_t eid) 
    : Component_t(eid) //llama al constructor padre
    {}
    static ComponentTypeID_t getComponentTypeID() noexcept {
        static ComponentTypeID_t typeID { ++nextTypeID };
        return typeID;
    }
};

} // namespace ECS
