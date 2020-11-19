#pragma once
#include <ecs/util/typealiases.hpp>
#include <ecs/util/keyboard.hpp>

template<typename GameCTX_t>
struct InputSystem_t {
    explicit InputSystem_t();

    bool update(GameCTX_t& g) const;
    bool isEscPressed();
private:
    static void onkeypress(KeySym k);
    static void onkeyrelease(KeySym k);
  
    inline static ECS::Keyboard_t ms_Keyboard {};
};
