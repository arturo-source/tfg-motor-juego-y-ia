#pragma once
#include <game/man/state.hpp>
#include <game/man/game.hpp>

struct ReplayGame_t : StateBase_t {
    explicit ReplayGame_t(StateManager_t& sm, std::string filename) : SM{sm} {
        ms_Keyboard = GM.Input.getKeyboard();
        read(filename);
    }

    void read(std::string& filename) {
        //Open BIN file
        std::ifstream file(filename.c_str(), std::ios::binary);
        if(!file) std::runtime_error("Can't open BIN file for read\n");

        //Calculate lenght
        std::size_t lenght = [&]() {
            file.seekg(0, std::ifstream::end);
            auto l { file.tellg() };
            file.seekg(0, std::ifstream::beg);
            return l;
        }();

        //Read all the file
        std::vector<char> filemem(lenght);
        auto* pfilemem = filemem.data();
        file.read(pfilemem, lenght);

        //Charge variables
        for(uint32_t i=0; i<lenght; i+=4) {
            KeysPressed_t kp {};
            std::memcpy(&kp, pfilemem + i, sizeof(kp));
            inputKeys.push_back(kp);
        }
        file.close();
        inputKeysIterator = inputKeys.begin();
    }

    void update() {
        if(!ms_Keyboard) std::runtime_error("Error manipulating keyboard\n");
        
        if( inputKeysIterator->up )   ms_Keyboard->keyPressed(XK_o);
        if( inputKeysIterator->down ) ms_Keyboard->keyPressed(XK_l);
        if( inputKeysIterator->up )   ms_Keyboard->keyPressed(XK_w);
        if( inputKeysIterator->down ) ms_Keyboard->keyPressed(XK_s);
        
        GM.update();
        
        if( ++inputKeysIterator == inputKeys.end() ) m_playing = false;
        else m_playing = GM.alive();
    }
    bool alive() { return m_playing; }

    StateManager_t& SM;
    GameManager_t GM { SM };
    inline static ECS::Keyboard_t* ms_Keyboard { nullptr };
    std::vector<KeysPressed_t> inputKeys;
    std::vector<KeysPressed_t>::iterator inputKeysIterator;
    bool m_playing { true };
};
