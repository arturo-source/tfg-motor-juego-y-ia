#pragma once

#include <cstdint>
#include <vector>
#include <ecs/util/typealiases.hpp>
#include <ecs/man/componentstorage.tpp>
#include <ecs/cmp/entity.hpp>

namespace ECS {
    
struct EntityManager_t {
      explicit EntityManager_t();

      Entity_t& createEntity();
      void destroyEntityByID(EntityID_t eid);

      const Entity_t* getEntityByID(EntityID_t eid) const;  
            Entity_t* getEntityByID(EntityID_t eid)      ; 

      const Vec_t<Entity_t>& getEntities() const { return m_Entity; }
            Vec_t<Entity_t>& getEntities()       { return m_Entity; }

      template<typename CMP_t>       CMP_t&        addComponent(Entity_t& e);
      template<typename CMP_t>       Entity_t*     getEntityPointerFromComponent(const CMP_t& cmp);
      template<typename CMP_t> const Entity_t*     getEntityPointerFromComponent(const CMP_t& cmp) const;
      template<typename CMP_t>       Vec_t<CMP_t>& getComponents() { return m_components.getComponents<CMP_t>(); }
      template<typename CMP_t> const Vec_t<CMP_t>& getComponents() const { return m_components.getComponents<CMP_t>(); }

      template<typename ReqCMP_t, typename CMP_t> ReqCMP_t* getRequiredComponent(const CMP_t& cmp);
      template<typename ReqCMP_t, typename CMP_t> const ReqCMP_t* getRequiredComponent(const CMP_t& cmp) const;

      static constexpr std::size_t kNUMINITIALENTITIES { 1000 };
private:
      Vec_t<Entity_t> m_Entity {};
      ComponentStorage_t m_components {kNUMINITIALENTITIES};
};

} // namespace ECS
