#include <ecs/util/keyboard.hpp>

namespace ECS {
    bool Keyboard_t::isKeyPressed (KeySym k) const noexcept {
        if(auto it = getIterator(k))
            return (*it)->second;
        return false;
    }

    const Keyboard_t::ConstOptIter Keyboard_t::getIterator(KeySym k) const noexcept {
        auto it = m_pressedKeys.find(k);
        if(it != m_pressedKeys.end())
            return it;
        return {};
    }
    
    Keyboard_t::OptIter Keyboard_t::getIterator(KeySym k) noexcept {
        auto it = m_pressedKeys.find(k);
        if(it != m_pressedKeys.end())
            return it;
        return {};
    }

} // namespace ECS
