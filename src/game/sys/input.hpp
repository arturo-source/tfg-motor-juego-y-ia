#pragma once
#include <ecs/util/typealiases.hpp>
#include <ecs/util/keyboard.hpp>
#include <game/cmp/input.hpp>

struct KeysPressed_t {
    uint8_t player_side;
    bool    up;
    bool    down;
};

template<typename GameCTX_t>
struct InputSystem_t {
    explicit InputSystem_t();

    std::string filename;

    bool update(GameCTX_t& g) const;
    bool isEscPressed();
    bool isPausePressed();
    
    void dumpBin(const InputComponent_t& inp) const;
    void dumpCSV(const InputComponent_t& inp) const;
    void initCSV(const std::string fname);
    ECS::Keyboard_t& getKeyboard() { return ms_Keyboard; }
private:
    static void onkeypress(KeySym k);
    static void onkeyrelease(KeySym k);

    inline static ECS::Keyboard_t ms_Keyboard {};
};
