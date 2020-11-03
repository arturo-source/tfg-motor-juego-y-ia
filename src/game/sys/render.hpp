#pragma once
#include <cstdint>
#include <memory>
#include <ecs/util/typealiases.hpp>
#include <ecs/cmp/entity.hpp>

template<typename GameCTX_t>
struct RenderSystem_t {
    explicit RenderSystem_t(uint32_t w, uint32_t h);
    ~RenderSystem_t();

    bool update(const GameCTX_t& g) const;
    void drawAllEntities(const GameCTX_t& g) const;

    //Constantes
    static constexpr uint32_t kB = 0x00444444;
private:
    constexpr uint32_t* getScreenXY(uint32_t x, uint32_t y) const {return m_framebuffer.get() + y*m_w + x;}
    const uint32_t m_w {0}, m_h{0};
    std::unique_ptr<uint32_t[]> m_framebuffer {nullptr};
};
