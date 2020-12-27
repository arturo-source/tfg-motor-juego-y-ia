#pragma once
#include <cstdint>
#include <memory>
#include <cmath>
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
    constexpr void drawSprite(uint32_t* screen, const uint32_t* sprite, const uint32_t w, const uint32_t h) const;
    constexpr uint32_t* getScreenXY(float x, float y) const;
    const uint32_t m_w {0}, m_h{0};
    std::unique_ptr<uint32_t[]> m_framebuffer {nullptr};
};
