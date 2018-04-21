#ifndef DRAW_HPP
#define DRAW_HPP

#include <switch.h>
#include "types.hpp"

extern const ffnt_header_t tahoma24_nxfnt;
extern const ffnt_header_t interuimedium42_nxfnt;
#define font24 &tahoma24_nxfnt
#define font42 &interuimedium42_nxfnt

extern u8* g_framebuf;
extern u32 g_framebufWidth;

// the following code is from nx-hbmenu
// https://github.com/switchbrew/nx-hbmenu/blob/master/common/common.h#L63

static inline u8 BlendColor(u32 src, u32 dst, u8 alpha)
{
    u8 one_minus_alpha = (u8)255 - alpha;
    return (dst*alpha + src*one_minus_alpha)/(u8)255;
}

static inline color_t MakeColor(u8 r, u8 g, u8 b, u8 a)
{
    color_t clr;
    clr.r = r;
    clr.g = g;
    clr.b = b;
    clr.a = a;
    return clr;
}

static inline void DrawPixel(u32 x, u32 y, color_t clr)
{
    if (x >= 1280 || y >= 720)
        return;
    u32 off = (y * g_framebufWidth + x)*4;
    g_framebuf[off] = BlendColor(g_framebuf[off], clr.r, clr.a); off++;
    g_framebuf[off] = BlendColor(g_framebuf[off], clr.g, clr.a); off++;
    g_framebuf[off] = BlendColor(g_framebuf[off], clr.b, clr.a); off++;
    g_framebuf[off] = 0xff;
}

static inline void Draw4PixelsRaw(u32 x, u32 y, color_t clr)
{
    if (x >= 1280 || y >= 720 || x > 1280-4)
        return;

    u32 color = clr.r | (clr.g<<8) | (clr.b<<16) | (0xff<<24);
    u128 val = color | ((u128)color<<32) | ((u128)color<<64) | ((u128)color<<96);
    u32 off = (y * g_framebufWidth + x)*4;
    *((u128*)&g_framebuf[off]) = val;
}

void rectangle(u32 x, u32 y, u16 w, u16 h, color_t color);
void DrawPixel(u32 x, u32 y, color_t clr);
void DrawText(const ffnt_header_t* font, u32 x, u32 y, color_t clr, const char* text);
void GetTextDimensions(const ffnt_header_t* font, const char* text, u32* width_out, u32* height_out);

#endif