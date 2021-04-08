#pragma once
#include <ecs/util/typealiases.hpp>
#include <ecs/util/keyboard.hpp>
#include <game/sys/artificialinteligence.hpp>
#include <game/util/gameobjectfactory.hpp>

template<typename GameCTX_t>
struct InputSystem_t {
    explicit InputSystem_t();

    void update(GameCTX_t& g) const;
    
    constexpr bool isKeyPressed(ECS::KeySym k) const {
        // ptc_process_events();
        return ms_Keyboard.isKeyPressed(k);
    }
    void setObjectFactory(const GameObjectFactory_t& gof) { m_GOFactory = &gof; }
    
    static ECS::Keyboard_t& getKeyboard() { return ms_Keyboard; }
private:
    static void onkeypress(ECS::KeySym k);
    static void onkeyrelease(ECS::KeySym k);

    inline static ECS::Keyboard_t ms_Keyboard {};
    const GameObjectFactory_t* m_GOFactory;
};
