#pragma once
#include <cstdint>
#include <memory>
#include <cmath>
#include <ecs/util/typealiases.hpp>
#include <ecs/cmp/entity.hpp>
#include <game/util/gameconfig.hpp>
#include <game/util/imguiutilities.hpp>

template<typename GameCTX_t>
struct RenderSystem_t {
    explicit RenderSystem_t(uint32_t w, uint32_t h);

    void update(const GameCTX_t& g) const;
    
    const ImGuiUtilities& getMenu() const noexcept { return igUtils; }
    const bool            shouldClose() const noexcept{ return igUtils.shouldClose(); }

    //Constantes
    static constexpr uint32_t backgroungColor = 0xff00897b;
private:
    void drawAllEntities(const GameCTX_t& g) const;
    constexpr void drawSprite(uint32_t* screen, const uint32_t* sprite, const uint32_t w, const uint32_t h) const;
    constexpr void drawSprite(uint32_t* screen, const unsigned char* sprite, const uint32_t w, const uint32_t h, const uint32_t color) const;
    constexpr uint32_t* getScreenXY(float x, float y) const;
    
    const uint32_t m_w {0}, m_h{0};
    std::unique_ptr<uint32_t[]> m_framebuffer {nullptr};
    ImGuiUtilities igUtils;
};
