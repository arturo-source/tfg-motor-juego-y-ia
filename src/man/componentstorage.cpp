#include <man/componentstorage.hpp>

namespace ECS {
    PhysicsComponent_t& ComponentStorage_t::createPhysicsComponent(EntityID_t eid) {
        auto& cmp = m_physicsComponents.emplace_back(eid);
        return cmp;
    }
    RenderComponent_t& ComponentStorage_t::createRenderComponent(EntityID_t eid) {
        auto& cmp = m_renderComponents.emplace_back(eid);
        return cmp;
    }
} // namespace ECS
