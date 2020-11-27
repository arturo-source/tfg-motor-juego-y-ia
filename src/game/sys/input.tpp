#include <game/sys/input.hpp>
#include <X11/X.h>
#include <X11/keysym.h>
extern "C" {
    #include <tinyPTC/src/linux/tinyptc.h>
}

template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::onkeypress(KeySym k) {
    ms_Keyboard.keyPressed(k);
}
template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::onkeyrelease(KeySym k) {
    ms_Keyboard.keyReleased(k);
}

template<typename GameCTX_t>
InputSystem_t<GameCTX_t>::InputSystem_t() {
    ptc_set_on_keypress( onkeypress );
    ptc_set_on_keyrelease( onkeyrelease );
    ms_Keyboard.reset();
}

template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::dumpBin(const InputComponent_t& inp) const {
    KeysPressed_t k {
        inp.getEntityID(),
        ms_Keyboard.isKeyPressed(inp.key_UP),
        ms_Keyboard.isKeyPressed(inp.key_DOWN)
    };
    std::ofstream file(filename.c_str(), std::ios::app | std::ios::binary);
    file.write(reinterpret_cast<const char*>(&k), sizeof(k));
    file.close();
}

template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::initCSV(const std::string fname) {
    filename = fname;
    std::ofstream file(filename.c_str(), std::ios::app);
    file << "EntityID;kUP;kDOWN\n";
    file.close();
}

template<typename GameCTX_t>
void InputSystem_t<GameCTX_t>::dumpCSV(const InputComponent_t& inp) const {
    std::ofstream file(filename.c_str(), std::ios::app);
    file << inp.getEntityID() << ";" << ms_Keyboard.isKeyPressed(inp.key_UP) << ";" << ms_Keyboard.isKeyPressed(inp.key_DOWN) << "\n";
    file.close();
}

template<typename GameCTX_t>
bool InputSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    ptc_process_events();

    for(auto& inp : g.template getComponents<InputComponent_t>()) {
        auto* e = g.getEntityByID(inp.getEntityID());
        if(e) {
            auto phy = e->template getComponent<PhysicsComponent_t>();
            if(phy) { // phy != nullptr
                phy->aceleration = 0;
                // phy->vx = 0;
                // if(ms_Keyboard.isKeyPressed(inp.key_LEFT )) phy->vx = -1;
                // if(ms_Keyboard.isKeyPressed(inp.key_RIGHT)) phy->vx = 1;
                if(ms_Keyboard.isKeyPressed(inp.key_UP   )) phy->aceleration = -1;
                    // if(phy->jumpIdx == phy->jumpTable.size()) 
                    //     phy->jumpIdx = 0;
                if(ms_Keyboard.isKeyPressed(inp.key_DOWN )) phy->aceleration = 1;
            }
        }
        dumpCSV(inp);
    }
    return true;
}

template<typename GameCTX_t>
bool InputSystem_t<GameCTX_t>::isEscPressed() {
    return ms_Keyboard.isKeyPressed(XK_Escape);
}

template<typename GameCTX_t>
bool InputSystem_t<GameCTX_t>::isPausePressed() {
    return ms_Keyboard.isKeyPressed(XK_space);
}