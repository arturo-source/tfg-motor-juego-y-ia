#include <unordered_map>
#include <ecs/util/keys.hpp>

namespace ECS {
struct Keyboard_t {
    using OptIter = std::optional<std::unordered_map<KeySym, bool>::iterator>;
    using ConstOptIter = std::optional<std::unordered_map<KeySym, bool>::const_iterator>;

    explicit Keyboard_t() = default;

    Keyboard_t(const Keyboard_t&)            = delete;
    Keyboard_t(Keyboard_t&&)                 = delete;
    Keyboard_t& operator=(const Keyboard_t&) = delete;
    Keyboard_t& operator=(Keyboard_t&&)      = delete;

    bool isKeyPressed (KeySym k) const noexcept;
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
    const ConstOptIter getIterator(KeySym k) const noexcept;
    OptIter getIterator(KeySym k) noexcept;
    std::unordered_map<KeySym, bool> m_pressedKeys {
        {Tab,   false},
        {Esc,   false},
        {Space, false},
        {Intro, false},
        {l,     false},
        {o,     false},
        {a,     false},
        {s,     false},
        {d,     false},
        {w,     false},
        {Up,    false},
        {Down,  false},
        {Left,  false},
        {Right, false}
    };
};

} // namespace ECS
