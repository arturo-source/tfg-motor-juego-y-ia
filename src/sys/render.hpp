#pragma once
#include <cstdint>
#include <memory>
#include <util/typealiases.hpp>
#include <cmp/entity.hpp>

namespace ECS {

//Forward declaration
struct EntityManager_t;
struct GameContext_t;


struct RenderSystem_t {
    explicit RenderSystem_t(uint32_t w, uint32_t h);
    ~RenderSystem_t();

    bool update(const GameContext_t& g) const;
    void drawAllEntities(const Vec_t<Entity_t>& entities) const;

    //Constantes
    static constexpr uint32_t kR = 0x00FF0000;
private:
    const uint32_t m_w {0}, m_h{0};
    std::unique_ptr<uint32_t[]> m_framebuffer {nullptr};
};

} //namespace ECS
