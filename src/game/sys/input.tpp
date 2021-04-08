#include <ecs/util/keys.hpp>
#include <game/sys/input.hpp>
#include <game/cmp/weapon.hpp>
#include <game/cmp/input.hpp>

template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::onkeypress(ECS::KeySym k) {
    ms_Keyboard.keyPressed(k);
}
template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::onkeyrelease(ECS::KeySym k) {
    ms_Keyboard.keyReleased(k);
}

template<typename GameCTX_t>
InputSystem_t<GameCTX_t>::InputSystem_t() {
    // ptc_set_on_keypress( onkeypress );
    // ptc_set_on_keyrelease( onkeyrelease );
    // ms_Keyboard.reset();
}

template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    if(!m_GOFactory) throw std::runtime_error("Missing game object factory.");
    // ptc_process_events();

    for(auto& inp : g.template getComponents<InputComponent_t>()) {
        auto phy = g.template getRequiredComponent<PhysicsComponent_t>(inp);
        if(phy) { // phy != nullptr
            if(ms_Keyboard.isKeyPressed(inp.key_DOWN)) phy->aceleration =  0.44;
            if(ms_Keyboard.isKeyPressed(inp.key_UP)  ) phy->aceleration = -0.44;

            auto weap = g.template getRequiredComponent<WeaponComponent_t>(inp);
            if(weap != nullptr && weap->shoot_cooldown == 0 && weap->bullets > 0
                && ms_Keyboard.isKeyPressed(inp.key_shoot)) 
            {
                m_GOFactory->createBullet(*phy, inp.side);
                weap->setCooldown();
                --(weap->bullets);
            }
        }
    }
}