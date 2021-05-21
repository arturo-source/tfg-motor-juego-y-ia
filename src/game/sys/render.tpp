#include <memory>
#include <algorithm>
#include <fstream>
#include <vector>
#include <ecs/man/entitymanager.hpp>
#include <game/sys/render.hpp>
#include <game/cmp/render.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/score.hpp>

template<typename GameCTX_t>
RenderSystem_t<GameCTX_t>::RenderSystem_t(uint32_t w, uint32_t h)
: m_w{w}, m_h{h}
, m_framebuffer{std::make_unique<uint32_t[]>(m_w*m_h)} 
, igUtils{m_w, m_h, m_framebuffer.get()}
{}

template<typename GameCTX_t>
constexpr uint32_t* RenderSystem_t<GameCTX_t>::getScreenXY(float x, float y) const {
    return m_framebuffer.get() + 
    static_cast<uint32_t>(std::round(y))*m_w + 
    static_cast<uint32_t>(std::round(x));
}

template<typename GameCTX_t>
void RenderSystem_t<GameCTX_t>::drawAllEntities(const GameCTX_t& g) const {
    auto screen = m_framebuffer.get();
    uint32_t left_score = 0, right_score = 0;

    auto drawEntity = [&](const RenderComponent_t& ren) {
        const auto* phy = g.template getRequiredComponent<PhysicsComponent_t>(ren);
        const auto* sco = g.template getRequiredComponent<ScoreComponent_t>(ren);
        if(phy) { //phy != null
            auto screen = getScreenXY(phy->x, phy->y);
            auto sprite = ren.sprite.data();
            
            drawSprite(screen, sprite, ren.w, ren.h);
            if(sco) {
                if(phy->x < m_w/2)      left_score  = sco->score;
                else if(phy->x > m_w/2) right_score = sco->score;
            }
        }
    };
    auto& rencmps = g.template getComponents<RenderComponent_t>();
    
    std::for_each(begin(rencmps), end(rencmps), drawEntity);
    igUtils.renderScoreboard(left_score, right_score);
}

template<typename GameCTX_t>
constexpr void RenderSystem_t<GameCTX_t>::drawSprite(uint32_t* screen, const uint32_t* sprite, const uint32_t w, const uint32_t h) const {
    for(uint32_t j=0; j<h; ++j) {
        for(uint32_t i=0; i<w; ++i) {
            if(*sprite & 0xFF000000)
                *screen = *sprite;
            ++sprite;
            ++screen;
        }
        screen += m_w - w;
    }
}

template<typename GameCTX_t>
constexpr void RenderSystem_t<GameCTX_t>::drawSprite(uint32_t* screen, const unsigned char* sprite, const uint32_t w, const uint32_t h, const uint32_t color) const {
    for(uint32_t j=0; j<h; ++j) {
        for(uint32_t i=0; i<w; ++i) {
            if(*sprite) *screen = color;
            ++sprite;
            ++screen;
        }
        screen += m_w - w;
    }
}

template<typename GameCTX_t>
void RenderSystem_t<GameCTX_t>::update(const GameCTX_t& g) const {
    auto screen = m_framebuffer.get();
    const auto size = m_w*m_h;
    std::fill(screen, screen + size, backgroungColor);
    igUtils.prerender();
    drawAllEntities(g);
    
    //Render framebuffer in OpenGL context
    igUtils.renderFrameBuffer();
    igUtils.postrender();
}