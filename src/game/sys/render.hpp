#pragma once
#define STB_TRUETYPE_IMPLEMENTATION
extern "C" {
    #include <stb_truetype/src/stb_truetype.h>
}
#include <cstdint>
#include <memory>
#include <cmath>
#include <ecs/util/typealiases.hpp>
#include <ecs/cmp/entity.hpp>

template<typename GameCTX_t>
struct RenderSystem_t {
    explicit RenderSystem_t(uint32_t w, uint32_t h);
    ~RenderSystem_t();

    void update(const GameCTX_t& g) const;
    void drawAllEntities(const GameCTX_t& g) const;
    
    void updateMenu() const;

    //Constantes
    static constexpr uint32_t backgroungColor = 0xff00897b;
private:
    constexpr void drawSprite(uint32_t* screen, const uint32_t* sprite, const uint32_t w, const uint32_t h) const;
    constexpr uint32_t* getScreenXY(float x, float y) const;
    constexpr void drawOption(const std::string text, uint32_t *screen, const uint32_t height, const uint32_t width) const;
    void drawAllOptions(uint32_t *screen) const;
    void loadFonts(const std::string_view filename);
    
    stbtt_fontinfo font;
    const uint32_t m_w {0}, m_h{0};
    std::unique_ptr<uint32_t[]> m_framebuffer {nullptr};
};
