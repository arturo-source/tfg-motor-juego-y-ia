extern "C" {
    #ifdef windows
    #include <tinyPTC/src/windows/tinyptc.h>
    #else
    #include <tinyPTC/src/linux/tinyptc.h>
    #endif
}
#include <memory>
#include <algorithm>
#include <fstream>
#include <vector>
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
    loadFonts("/usr/share/fonts/TTF/OpenSans-Regular.ttf");
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
void RenderSystem_t<GameCTX_t>::update(const GameCTX_t& g) const {
    auto screen = m_framebuffer.get();
    const auto size = m_w*m_h;
    std::fill(screen, screen + size, backgroungColor);
    drawAllEntities(g);
    
    ptc_update(screen);
}

template<typename GameCTX_t>
void RenderSystem_t<GameCTX_t>::updateMenu(const std::vector<std::string> options, uint8_t selected_option) const {
    constexpr uint32_t text_height=60, text_width=400;
    const uint32_t options_per_page { m_h/text_height }, page { selected_option/options_per_page };

    auto screen = m_framebuffer.get();
    const auto size = m_w*m_h;

    std::fill(screen, screen + size, backgroungColor);
    for(uint8_t i = page * options_per_page; i < (page+1) * options_per_page && i < options.size(); ++i) {
        if(i != selected_option) 
            drawOption(options[i], 0xff000000, screen + (m_w-text_width)/2, text_height, text_width);
        else
            drawOption(options[i], 0xff56c8d8, screen + (m_w-text_width)/2, text_height, text_width);
            
        screen += m_w * text_height;
    }

    ptc_update(m_framebuffer.get());
}

template<typename GameCTX_t>
constexpr void RenderSystem_t<GameCTX_t>::drawOption(const std::string text, const uint32_t color, uint32_t *screen, const uint32_t height, const uint32_t width) const {
    unsigned char bitmap[height][width] = {0};

    int i,j,ascent,baseline,ch=0;
    float scale, xpos=2;

    scale = stbtt_ScaleForPixelHeight(&font, height);
    stbtt_GetFontVMetrics(&font, &ascent,0,0);
    baseline = (int) (ascent*scale);

    while(text[ch]) {
        int advance,lsb,x0,y0,x1,y1;
        float x_shift = xpos - (float) floor(xpos);
        stbtt_GetCodepointHMetrics(&font, text[ch], &advance, &lsb);
        stbtt_GetCodepointBitmapBoxSubpixel(&font, text[ch], scale,scale,x_shift,0, &x0,&y0,&x1,&y1);
        stbtt_MakeCodepointBitmapSubpixel(&font, &bitmap[baseline + y0][(int) xpos + x0], x1-x0,y1-y0, width, scale,scale,x_shift,0, text[ch]);
        xpos += (advance * scale);
        if (text[ch+1])
            xpos += scale*stbtt_GetCodepointKernAdvance(&font, text[ch],text[ch+1]);
        ++ch;
    }

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j, ++screen) {
            if(bitmap[i][j]) *screen = color;
        }
        screen += m_w - width;
    }
}

template<typename GameCTX_t>
void RenderSystem_t<GameCTX_t>::loadFonts(const std::string_view filename) {
    std::ifstream file(filename.data(), std::ios::binary);
    if(!file.is_open()) throw std::runtime_error("Error reading fonts file.");
    
    static const std::vector<unsigned char> filevec(
        std::istreambuf_iterator<char>{file},
        std::istreambuf_iterator<char>{}
    );
    stbtt_InitFont(&font, filevec.data(), stbtt_GetFontOffsetForIndex(filevec.data(),0));
}