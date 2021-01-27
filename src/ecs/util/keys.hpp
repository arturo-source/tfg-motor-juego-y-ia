#pragma once
#ifndef windows
extern "C" {
    #include <tinyPTC/src/linux/tinyptc.h>
}
#endif

namespace ECS {
using Key_t = unsigned long;
#ifdef windows
    constexpr Key_t Tab   = '\t';
    constexpr Key_t Esc   = '\e';
    constexpr Key_t Space = ' ';
    constexpr Key_t Intro = 13;
    constexpr Key_t l     = 'L';
    constexpr Key_t o     = 'O';
    constexpr Key_t a     = 'A';
    constexpr Key_t s     = 'S';
    constexpr Key_t d     = 'D';
    constexpr Key_t w     = 'W';
    constexpr Key_t Up    = 38;
    constexpr Key_t Down  = 40;
    constexpr Key_t Left  = 37;
    constexpr Key_t Right = 39;
#else
    constexpr Key_t Tab   = XK_Tab;
    constexpr Key_t Esc   = XK_Escape;
    constexpr Key_t Space = XK_space;
    constexpr Key_t Intro = XK_Return;
    constexpr Key_t l     = XK_l;
    constexpr Key_t o     = XK_o;
    constexpr Key_t a     = XK_a;
    constexpr Key_t s     = XK_s;
    constexpr Key_t d     = XK_d;
    constexpr Key_t w     = XK_w;
    constexpr Key_t Up    = XK_Up;
    constexpr Key_t Down  = XK_Down;
    constexpr Key_t Left  = XK_Left;
    constexpr Key_t Right = XK_Right;
#endif
    
} // namespace ECS
