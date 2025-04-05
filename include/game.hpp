#ifndef GAME_HPP
#define GAME_HPP

#include <algorithm>
#include <cstring>
#include <cmath>
#include <array>
#include <ctime>
#include <vector>
#include "cell.hpp"
#include <switch.h>

#define FB_WIDTH 1280
#define FB_HEIGHT 720

namespace Grid
{
    void addNumber(void);
    void slide(u8 row);
    void combine(u8 row);
    void operate(void);
    void flip(void);
    void transpose(void);
}

namespace Game 
{
    void init(void);
    void scanInput(u64);
    bool gameWon(void);
    bool gameOver(void);
    void saveState(void);
    void loadState(void);
    void show(Framebuffer* fb);
}

#endif