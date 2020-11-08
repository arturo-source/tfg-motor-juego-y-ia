#include <game/sys/input.hpp>
#include <game/cmp/input.hpp>
#include <X11/X.h>
#include <X11/keysym.h>
extern "C" {
    #include <tinyPTC/src/linux/tinyptc.h>
}

template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::onkeypress(KeySym k) {
    ms_Keyboard.keyPressed(k);
}
template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::onkeyrelease(KeySym k) {
    ms_Keyboard.keyReleased(k);
}

template<typename GameCTX_t>
InputSystem_t<GameCTX_t>::InputSystem_t() {
    ptc_set_on_keypress( onkeypress );
    ptc_set_on_keyrelease( onkeyrelease );
}

template<typename GameCTX_t>
bool InputSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    ptc_process_events();

    for(auto& inp : g.template getComponents<InputComponent_t>()) {
        auto* e = g.getEntityByID(inp.getEntityID());
        if(e) {
            auto phy = e->template getComponent<PhysicsComponent_t>();
            if(phy) { // phy != nullptr
                phy->vx = phy->vy = 0;
                if(ms_Keyboard.isKeyPressed(inp.key_LEFT )) phy->vx = -1;
                if(ms_Keyboard.isKeyPressed(inp.key_RIGHT)) phy->vx = 1;
                if(ms_Keyboard.isKeyPressed(inp.key_UP   )) phy->vy = -1;
                if(ms_Keyboard.isKeyPressed(inp.key_DOWN )) phy->vy = 1;
            }
        }
    }
    return true;
}
