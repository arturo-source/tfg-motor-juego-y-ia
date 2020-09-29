#pragma once

#include <cstdint>
#include <vector>
#include <util/typealiases.hpp>
#include <util/gamecontext.hpp>
#include <man/componentstorage.hpp>

namespace ECS {
    
struct EntityManager_t : public GameContext_t {
    static constexpr std::size_t kNUMINITIALENTITIES { 1000 };

    explicit EntityManager_t();
    void createEntity(uint32_t x, uint32_t y, const std::string_view filename);
    const Vec_t<Entity_t>& getEntities() const override { return m_Entity; }
          Vec_t<Entity_t>& getEntities()       override { return m_Entity; }

    const std::vector<PhysicsComponent_t>& getPhysicsComponents() const override { return m_components.getPhysicsComponents(); }
          std::vector<PhysicsComponent_t>& getPhysicsComponents()       override { return m_components.getPhysicsComponents(); }

private:
    Vec_t<Entity_t> m_Entity {};
    ComponentStorage_t m_components {kNUMINITIALENTITIES};
};

} // namespace ECS
