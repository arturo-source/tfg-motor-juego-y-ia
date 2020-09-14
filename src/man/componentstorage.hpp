#pragma once
#include <vector>
#include <cmp/physics.hpp>

namespace ECS {

struct ComponentStorage_t {
    ComponentStorage_t() = delete;
    explicit ComponentStorage_t(std::size_t initialsize) {
        m_physicsComponents.reserve(initialsize);
    }

    ComponentStorage_t(const ComponentStorage_t&) = delete;
    ComponentStorage_t(ComponentStorage_t&&) = delete;
    ComponentStorage_t operator=(const ComponentStorage_t&) = delete;
    ComponentStorage_t operator=(ComponentStorage_t&&) = delete;

    PhysicsComponent_t& createPhysicsComponent();

    const std::vector<PhysicsComponent_t>& getPhysicsComponents() const { return m_physicsComponents; }
          std::vector<PhysicsComponent_t>& getPhysicsComponents()       { return m_physicsComponents; }

private:
    std::vector<PhysicsComponent_t> m_physicsComponents {};
};

} // namespace ECS
