#pragma once
#include <game/man/state.hpp>
#include <game/man/game.hpp>

struct ReplayGame_t : StateBase_t {
    explicit ReplayGame_t(StateManager_t& sm, std::string filename) 
    : SM{sm}, ms_Keyboard { GM.getKeyboard() } 
    { readCSV(filename); }

    void readCSV(std::string& filename) {
        std::ifstream file(filename.c_str(), std::ios::binary);
        if(!file) std::runtime_error("Can't open BIN file for read\n");
        
        std::string line;
        std::getline(file,line); //Erase the first line of csv
        while(std::getline(file,line)) {
            std::stringstream lineStream(line);
            std::string side;
            std::string kUP;
            std::string kDOWN;
            std::getline(lineStream,side, ';');
            std::getline(lineStream,kUP, ';');
            std::getline(lineStream,kDOWN, ';');

            KeysPressed_t kp {
                static_cast<uint8_t>( atoi(side.c_str()) ),
                static_cast<bool>( atoi(kUP.c_str()) ),
                static_cast<bool>( atoi(kDOWN.c_str()) )
            };
            inputKeys.push_back(kp);
        }
        file.close();
        inputKeysIterator = inputKeys.begin();
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

    void pressKeys() const {
        if( inputKeysIterator->player_side & InputComponent_t::S_Right) {
            if( inputKeysIterator->up )   ms_Keyboard.keyPressed(XK_o);
            if( inputKeysIterator->down ) ms_Keyboard.keyPressed(XK_l);
        } else if( inputKeysIterator->player_side & InputComponent_t::S_Left ) {
            if( inputKeysIterator->up )   ms_Keyboard.keyPressed(XK_w);
            if( inputKeysIterator->down ) ms_Keyboard.keyPressed(XK_s);
        }
    }

    void update() {
        pressKeys(); //Move player 1
        if( ++inputKeysIterator == inputKeys.end() ) {
            m_playing = false;
            return;
        }
        pressKeys(); //Move player 2
        
        GM.update();
        
        if( ++inputKeysIterator == inputKeys.end() ) m_playing = false;
        else m_playing = GM.alive();
        
        //Release all keys
        ms_Keyboard.keyReleased(XK_o);
        ms_Keyboard.keyReleased(XK_l);
        ms_Keyboard.keyReleased(XK_w);
        ms_Keyboard.keyReleased(XK_s);
    }
    bool alive() { return m_playing; }

    StateManager_t& SM;
    GameManager_t GM { SM };
    ECS::Keyboard_t& ms_Keyboard;
    std::vector<KeysPressed_t> inputKeys {};
    std::vector<KeysPressed_t>::iterator inputKeysIterator;
    bool m_playing { true };
};
