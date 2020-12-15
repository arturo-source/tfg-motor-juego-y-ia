#pragma once
#include <ecs/util/typealiases.hpp>
#include <ecs/util/keyboard.hpp>
#include <game/cmp/input.hpp>
#include <game/sys/artificialinteligence.hpp>

template<typename GameCTX_t>
struct InputSystem_t {
    explicit InputSystem_t(ArtificialInteligenceSystem_t<GameCTX_t>& ai);

    bool update(GameCTX_t& g) const;
    bool isEscPressed();
    bool isPausePressed();
    
    void initCSV(const std::string fname);
    ECS::Keyboard_t& getKeyboard() { return ms_Keyboard; }
private:
    static void onkeypress(KeySym k);
    static void onkeyrelease(KeySym k);

    inline static ECS::Keyboard_t ms_Keyboard {};
    ArtificialInteligenceSystem_t<GameCTX_t>& AI_system {nullptr};
};
