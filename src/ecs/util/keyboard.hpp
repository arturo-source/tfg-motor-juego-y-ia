#include <X11/X.h>
#include <X11/keysym.h>
#include <unordered_map>

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
private:
    // const OptIter getIterator(KeySym k) const noexcept;
    OptIter getIterator(KeySym k) noexcept;
    std::unordered_map<KeySym, bool> m_pressedKeys {
        {XK_BackSpace, false},
        {XK_Tab, false},
        {XK_a, false},
        {XK_s, false},
        {XK_d, false},
        {XK_w, false}
    };   
};

} // namespace ECS
