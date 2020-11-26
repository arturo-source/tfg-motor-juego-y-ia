#pragma once
#include <ecs/util/typealiases.hpp>
#include <ecs/util/keyboard.hpp>
#include <game/cmp/input.hpp>

struct KeysPressed_t {
    ECS::EntityID_t player_ID;
    bool     up;
    bool     down;
};

template<typename GameCTX_t>
struct InputSystem_t {
    explicit InputSystem_t();

    static constexpr std::size_t KMaxInputsStored = 1000;
    std::string filename;

    bool update(GameCTX_t& g) const;
    bool isEscPressed();
    bool isPausePressed();
    
    void addInput(const InputComponent_t& inp) const;
    void dump() const;
    ECS::Keyboard_t* getKeyboard() { return &ms_Keyboard; }
private:
    static void onkeypress(KeySym k);
    static void onkeyrelease(KeySym k);

    //PROBABLEMENTE ESTÁ MAL DECIDIR HACERLOS MUTABLES
    mutable uint32_t piputKeys { 0 };
    mutable std::vector<KeysPressed_t> inputKeys { KMaxInputsStored };
    inline static ECS::Keyboard_t ms_Keyboard {};
};
