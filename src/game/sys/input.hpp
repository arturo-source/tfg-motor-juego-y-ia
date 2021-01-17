#pragma once
#include <ecs/util/typealiases.hpp>
#include <ecs/util/keyboard.hpp>
#include <game/sys/artificialinteligence.hpp>
#include <game/util/gameobjectfactory.hpp>

template<typename GameCTX_t>
struct InputSystem_t {
    explicit InputSystem_t(GameObjectFactory_t& GOFactory);

    void update(GameCTX_t& g) const;
    bool isEscPressed();
    
    ECS::Keyboard_t& getKeyboard() { return ms_Keyboard; }
private:
    static void onkeypress(KeySym k);
    static void onkeyrelease(KeySym k);

    inline static ECS::Keyboard_t ms_Keyboard {};
    const GameObjectFactory_t& m_GOFactory;
};
