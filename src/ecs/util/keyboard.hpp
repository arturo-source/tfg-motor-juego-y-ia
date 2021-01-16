extern "C" {
    #ifdef windows
    #include <tinyPTC/src/windows/tinyptc.h>
    #else
    #include <tinyPTC/src/linux/tinyptc.h>
    #endif
}
#include <unordered_map>
#include <iostream>

namespace ECS {
struct Keyboard_t {
    using OptIter = std::optional<std::unordered_map<KeySym, bool>::iterator>;

    explicit Keyboard_t() = default;

    Keyboard_t(const Keyboard_t&)            = delete;
    Keyboard_t(Keyboard_t&&)                 = delete;
    Keyboard_t& operator=(const Keyboard_t&) = delete;
    Keyboard_t& operator=(Keyboard_t&&)      = delete;

    bool isKeyPressed (KeySym k) noexcept;
    void keyPressed(KeySym k) noexcept {
        if(auto it = getIterator(k))
            (*it)->second = true;
    }
    void keyReleased(KeySym k) noexcept {
        if(auto it = getIterator(k))
            (*it)->second = false;
    }
    void reset() noexcept {
        for(auto& [_, st] : m_pressedKeys)
            st = false;
    }
private:
    // const OptIter getIterator(KeySym k) const noexcept;
    OptIter getIterator(KeySym k) noexcept;
    std::unordered_map<KeySym, bool> m_pressedKeys {
        #ifdef windows
        {'\t', false},
        {'\e', false},
        {' ',  false},
        {'L',  false},
        {'O',  false},
        {'A',  false},
        {'S',  false},
        {'D',  false},
        {'W',  false}
        #else
        {XK_Tab,    false},
        {XK_Escape, false},
        {XK_space,  false},
        {XK_l,      false},
        {XK_o,      false},
        {XK_a,      false},
        {XK_s,      false},
        {XK_d,      false},
        {XK_w,      false}
        #endif
        
    };
};

} // namespace ECS
