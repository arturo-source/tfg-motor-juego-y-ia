#include <ecs/util/keyboard.hpp>

namespace ECS {
    bool Keyboard_t::isKeyPressed(Key_t k) const noexcept {
        if(auto it = getIterator(k))
            return (*it)->second;
        return false;
    }

    const Keyboard_t::ConstOptIter Keyboard_t::getIterator(Key_t k) const noexcept {
        auto it = m_pressedKeys.find(k);
        if(it != m_pressedKeys.end())
            return it;
        return {};
    }
    
    Keyboard_t::OptIter Keyboard_t::getIterator(Key_t k) noexcept {
        auto it = m_pressedKeys.find(k);
        if(it != m_pressedKeys.end())
            return it;
        return {};
    }

} // namespace ECS
