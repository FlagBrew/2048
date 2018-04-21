#ifndef GAME_HPP
#define GAME_HPP

#include <algorithm>
#include <cstring>
#include <cmath>
#include <array>
#include <ctime>
#include <vector>
#include "cell.hpp"

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
    void scanInput(void);
    bool gameWon(void);
    bool gameOver(void);
    void saveState(void);
    void loadState(void);
    void show(void);
}

#endif