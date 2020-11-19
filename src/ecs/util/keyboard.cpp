#include <ecs/util/keyboard.hpp>

namespace ECS {
    bool Keyboard_t::isKeyPressed (KeySym k) noexcept {
        if(auto it = getIterator(k))
            return (*it)->second;
        return false;
    }
    bool Keyboard_t::isEscPressed () noexcept {
        return isKeyPressed(XK_Escape);
    }

    Keyboard_t::OptIter Keyboard_t::getIterator(KeySym k) noexcept {
        auto it = m_pressedKeys.find(k);
        if(it != m_pressedKeys.end())
            return it;
        return {};
    }
    //El pr.retroman dijo que hay que hacer la version const
    //de esta función
    // Keyboard_t::OptIter Keyboard_t::getIterator(KeySym k) noexcept {
    //     auto optptr = const_cast<const Keyboard_t*>(this)->getIterator(k);
    //     return const_cast<OptIter>(optptr);
    // }

} // namespace ECS
