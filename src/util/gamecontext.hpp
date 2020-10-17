#pragma once

#include <util/typealiases.hpp>
#include <cmp/physics.hpp>
#include <cmp/entity.hpp>
#include <cmp/input.hpp>

namespace ECS {
struct GameContext_t {
    virtual ~GameContext_t() = default;
    
    virtual const Entity_t* getEntityByID(EntityID_t eid) const = 0;  
    virtual       Entity_t* getEntityByID(EntityID_t eid)       = 0; 

    virtual const Vec_t<Entity_t>& getEntities() const = 0;  
    virtual       Vec_t<Entity_t>& getEntities()       = 0;  

    virtual const std::vector<PhysicsComponent_t>& getPhysicsComponents() const = 0;
    virtual       std::vector<PhysicsComponent_t>& getPhysicsComponents()       = 0;

    virtual const std::vector<RenderComponent_t>& getRenderComponents() const = 0;
    virtual       std::vector<RenderComponent_t>& getRenderComponents()       = 0;

    virtual const std::vector<InputComponent_t>& getInputComponents() const = 0;
    virtual       std::vector<InputComponent_t>& getInputComponents()       = 0;

};
 
} // namespace ECS
