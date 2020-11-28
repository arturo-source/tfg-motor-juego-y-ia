extern "C" {
    #include <tinyPTC/src/linux/tinyptc.h>
}
#include <memory>
#include <algorithm>
#include <ecs/man/entitymanager.hpp>
#include <game/sys/render.hpp>
#include <game/cmp/render.hpp>
#include <game/cmp/physics.hpp>

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
        if(phy) { //phy != null
            auto screen = getScreenXY(phy->x, phy->y);
            auto sprite_it = begin(ren.sprite);

            uint32_t w {ren.w};
            for(uint32_t j=0; j<ren.h; ++j) {
                for(uint32_t i=0; i<w; ++i) {
                    if(*sprite_it & 0xFF000000)
                        *screen = *sprite_it;
                    ++sprite_it;
                    ++screen;
                }
                sprite_it += ren.w - w;
                screen    += m_w - w;
            }
        }
    };
    auto& rencmps = g.template getComponents<RenderComponent_t>();
    
    std::for_each(begin(rencmps), end(rencmps), drawEntity);
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
