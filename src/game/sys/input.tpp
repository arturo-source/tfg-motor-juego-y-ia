#include <game/sys/input.hpp>
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
    ms_Keyboard.reset();
}

template<typename GameCTX_t>
bool InputSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    ptc_process_events();

    for(auto& inp : g.template getComponents<InputComponent_t>()) {
        auto phy = g.template getRequiredComponent<PhysicsComponent_t>(inp);
        auto pc = g.template getRequiredComponent<PerceptronComponent_t>(inp);
        if(phy) { // phy != nullptr
            phy->aceleration = 0;
            if(pc) { // pc != nullptr
                if(pc->keyDOWN_pressed && !pc->keyUP_pressed) phy->aceleration += 0.44;
                if(pc->keyUP_pressed && !pc->keyDOWN_pressed) phy->aceleration -= 0.44;
            } else {
                if(ms_Keyboard.isKeyPressed(inp.key_DOWN)) phy->aceleration += 0.44;
                if(ms_Keyboard.isKeyPressed(inp.key_UP)  ) phy->aceleration -= 0.44;
            }
        }
    }
    return true;
}

template<typename GameCTX_t>
bool InputSystem_t<GameCTX_t>::isEscPressed() {
    return ms_Keyboard.isKeyPressed(XK_Escape);
}

template<typename GameCTX_t>
bool InputSystem_t<GameCTX_t>::isPausePressed() {
    return ms_Keyboard.isKeyPressed(XK_space);
}