#ifndef CELL_HPP
#define CELL_HPP

#include <switch.h>
#include <unordered_map>
#include <string>
#include "draw.hpp"

extern const u32 g_l;
extern const u32 g_spacing;
extern const u32 g_xstart;
extern const u32 g_ystart;

class Cell
{
public:
    Cell(void) : n{0} { };
    Cell(u32 n) : n{n} { };
    ~Cell(void) { };

    void value(u32 m);
    u32 value(void);
    void show(u8 i, u8 j);

private:
    u32 n; // number
};

#endif