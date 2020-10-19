extern "C" {
    #include <tinyPTC/src/tinyptc.h>
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
void RenderSystem_t<GameCTX_t>::drawAllEntities(const GameCTX_t& g) const {
    auto screen = m_framebuffer.get();

    auto getScreenXY = [&](uint32_t x, uint32_t y) {
        return screen + y*m_w + x;
    };
    auto drawEntity = [&](const RenderComponent_t& rc) {
        auto eptr = g.getEntityByID(rc.getEntityID());

        if(eptr) { //eptr != nullptr
            auto phy = eptr->template getComponent<PhysicsComponent_t>();
            auto ren = eptr->template getComponent<RenderComponent_t>();
            if(phy && ren) {
                auto screen = getScreenXY(phy->x, phy->y);
                auto sprite_it = begin(ren->sprite);
                for(uint32_t y = 0; y < ren->h; ++y) {
                    std::copy(sprite_it, sprite_it + ren->w, screen);
                    sprite_it += ren->w;
                    screen += m_w;
                }
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
    std::fill(screen, screen + size, kR);
    drawAllEntities(g);
    
    ptc_update(screen);

    return !ptc_process_events();
}
