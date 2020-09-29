#include <man/entitymanager.hpp>
#include <algorithm>
#include <cmp/physics.hpp>
#include <string_view>

namespace ECS {

    EntityManager_t::EntityManager_t() {
        m_Entity.reserve(kNUMINITIALENTITIES);
    }
    void EntityManager_t::createEntity(uint32_t x, uint32_t y, const std::string_view filename) {
        auto& e = m_Entity.emplace_back(); // leer filename¡¡
        auto& ph = m_components.createPhysicsComponent(e.entityID);
        auto& rn = m_components.createRenderComponent(e.entityID);
        rn.loadFromFile(filename);
        e.phy = &ph;
        e.ren = &rn;
        ph.x = x; ph.y = y;
        // std::fill(begin(e.sprite), end(e.sprite), color);
    }
    
} // namespace ECS
