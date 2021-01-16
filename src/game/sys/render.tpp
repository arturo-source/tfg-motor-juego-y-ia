extern "C" {
    #ifdef windows
    #include <tinyPTC/src/windows/tinyptc.h>
    #else
    #include <tinyPTC/src/linux/tinyptc.h>
    #endif
}
#include <memory>
#include <algorithm>
#include <ecs/man/entitymanager.hpp>
#include <game/sys/render.hpp>
#include <game/cmp/render.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/score.hpp>
#include <game/util/numbers.hpp>

template<typename GameCTX_t>
RenderSystem_t<GameCTX_t>::RenderSystem_t(uint32_t w, uint32_t h)
: m_w{w}, m_h{h}
, m_framebuffer{std::make_unique<uint32_t[]>(m_w*m_h)} 
{
    ptc_open("window", w, h);
}
template<typename GameCTX_t>
RenderSystem_t<GameCTX_t>::~RenderSystem_t() {
    ptc_close();
}

template<typename GameCTX_t>
constexpr uint32_t* RenderSystem_t<GameCTX_t>::getScreenXY(float x, float y) const {
    return m_framebuffer.get() + 
    static_cast<uint32_t>(std::round(y))*m_w + 
    static_cast<uint32_t>(std::round(x));
}

template<typename GameCTX_t>
void RenderSystem_t<GameCTX_t>::drawAllEntities(const GameCTX_t& g) const {
    auto screen = m_framebuffer.get();

    auto drawEntity = [&](const RenderComponent_t& ren) {
        const auto* phy = g.template getRequiredComponent<PhysicsComponent_t>(ren);
        const auto* sco = g.template getRequiredComponent<ScoreComponent_t>(ren);
        if(phy) { //phy != null
            auto screen = getScreenXY(phy->x, phy->y);
            auto sprite = ren.sprite.data();
            
            drawSprite(screen, sprite, ren.w, ren.h);
            if(sco) {
                screen = getScreenXY(phy->x, 10);
                drawSprite(screen, RenderNumbers::numbers[sco->score], 6, 6);
            }
        }
    };
    auto& rencmps = g.template getComponents<RenderComponent_t>();
    
    std::for_each(begin(rencmps), end(rencmps), drawEntity);
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
bool RenderSystem_t<GameCTX_t>::update(const GameCTX_t& g) const {
    auto screen = m_framebuffer.get();
    const auto size = m_w*m_h;
    std::fill(screen, screen + size, kB);
    drawAllEntities(g);
    
    ptc_update(screen);

    return !ptc_process_events();
}
