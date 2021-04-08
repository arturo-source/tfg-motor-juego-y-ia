#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ECS {
using KeySym = unsigned long;
#ifdef windows
    constexpr KeySym Tab   = '\t';
    constexpr KeySym Esc   = '\e';
    constexpr KeySym Space = ' ';
    constexpr KeySym Intro = 13;
    constexpr KeySym l     = 'L';
    constexpr KeySym o     = 'O';
    constexpr KeySym a     = 'A';
    constexpr KeySym s     = 'S';
    constexpr KeySym d     = 'D';
    constexpr KeySym w     = 'W';
    constexpr KeySym Up    = 38;
    constexpr KeySym Down  = 40;
    constexpr KeySym Left  = 37;
    constexpr KeySym Right = 39;
#else
    constexpr KeySym Tab   = GLFW_KEY_TAB;
    constexpr KeySym Esc   = GLFW_KEY_ESCAPE;
    constexpr KeySym Space = GLFW_KEY_SPACE;
    constexpr KeySym Intro = GLFW_KEY_ENTER;
    constexpr KeySym l     = GLFW_KEY_L;
    constexpr KeySym o     = GLFW_KEY_O;
    constexpr KeySym a     = GLFW_KEY_A;
    constexpr KeySym s     = GLFW_KEY_S;
    constexpr KeySym d     = GLFW_KEY_D;
    constexpr KeySym w     = GLFW_KEY_W;
    constexpr KeySym Up    = GLFW_KEY_UP;
    constexpr KeySym Down  = GLFW_KEY_DOWN;
    constexpr KeySym Left  = GLFW_KEY_LEFT;
    constexpr KeySym Right = GLFW_KEY_RIGHT;
#endif
    
} // namespace ECS
