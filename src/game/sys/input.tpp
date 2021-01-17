#ifdef windows
#include <windows.h>
#else
#include <X11/keysymdef.h>
#endif
#include <game/sys/input.hpp>
#include <game/cmp/weapon.hpp>
#include <game/cmp/input.hpp>

template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::onkeypress(KeySym k) {
    ms_Keyboard.keyPressed(k);
}
template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::onkeyrelease(KeySym k) {
    ms_Keyboard.keyReleased(k);
}

template<typename GameCTX_t>
InputSystem_t<GameCTX_t>::InputSystem_t(GameObjectFactory_t& GOFactory) : m_GOFactory{GOFactory} {
    ptc_set_on_keypress( onkeypress );
    ptc_set_on_keyrelease( onkeyrelease );
    ms_Keyboard.reset();
}

template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
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

                auto weap = g.template getRequiredComponent<WeaponComponent_t>(inp);
                if(weap != nullptr && weap->shoot_cooldown == 0 && weap->bullets > 0
                   && ms_Keyboard.isKeyPressed(inp.key_shoot)) 
                {
                    m_GOFactory.createBullet(*phy, inp.side);
                    weap->setCooldown();
                    --(weap->bullets);
                }
            }
        }
    }
}

template<typename GameCTX_t>
bool InputSystem_t<GameCTX_t>::isEscPressed() {
    #ifdef windows
    return ms_Keyboard.isKeyPressed('\e');
    #else
    return ms_Keyboard.isKeyPressed(XK_Escape);
    #endif
}