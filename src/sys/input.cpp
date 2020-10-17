#include <sys/input.hpp>
#include <util/gamecontext.hpp>
#include <X11/X.h>
#include <X11/keysym.h>
extern "C" {
    #include <tinyPTC/src/tinyptc.h>
}

namespace ECS {
    void InputSystem_t::onkeypress(KeySym k) {
        ms_Keyboard.keyPressed(k);
    }
    void InputSystem_t::onkeyrelease(KeySym k) {
        ms_Keyboard.keyReleased(k);
    }

    InputSystem_t::InputSystem_t() {
        ptc_set_on_keypress( onkeypress );
        ptc_set_on_keyrelease( onkeyrelease );
    }

    bool InputSystem_t::update(GameContext_t& g) const {
        ptc_process_events();

        for(auto& inp : g.getInputComponents()) {
            auto* e = g.getEntityByID(inp.getEntityID());
            if(e && e->phy) {
                auto& phy = *(e->phy);
                phy.vx = phy.vy = 0;
                if(ms_Keyboard.isKeyPressed(inp.key_LEFT )) phy.vx = -1;
                if(ms_Keyboard.isKeyPressed(inp.key_RIGHT)) phy.vx = 1;
                if(ms_Keyboard.isKeyPressed(inp.key_UP   )) phy.vy = -1;
                if(ms_Keyboard.isKeyPressed(inp.key_DOWN )) phy.vy = 1;
            }
        }
        return true;
    }
    
} // namespace ECS
