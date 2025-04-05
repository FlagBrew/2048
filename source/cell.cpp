#include "cell.hpp"

color_t color_dark                                   = MakeColor(119, 110, 101, 255);
color_t color_light                                  = MakeColor(249, 246, 242, 255);
static const std::unordered_map<u32, color_t> colors = {{0, MakeColor(205, 192, 180, 255)}, {2, MakeColor(245, 245, 245, 255)},
    {4, MakeColor(245, 245, 220, 255)}, {8, MakeColor(255, 100, 122, 255)}, {16, MakeColor(255, 127, 80, 255)}, {32, MakeColor(255, 99, 71, 255)},
    {64, MakeColor(255, 0, 0, 255)}, {128, MakeColor(255, 240, 96, 255)}, {256, MakeColor(240, 224, 80, 255)}, {512, MakeColor(240, 224, 16, 255)},
    {1024, MakeColor(240, 208, 0, 255)}, {2048, MakeColor(240, 192, 0, 255)}, {4096, MakeColor(0, 0, 0, 255)}};

void Cell::value(u32 m)
{
    n = m;
}

u32 Cell::value(void)
{
    return n;
}

void Cell::show(u8 i, u8 j)
{
    auto t        = colors.find(n);
    color_t color = t == colors.end() ? colors.find(4096)->second : t->second;
    rectangle(g_xstart + (g_l + g_spacing) * j, g_ystart + (g_l + g_spacing) * i, g_l, g_l, color);

    if (n != 0) {
        u32 number_w;
        const char* text = std::to_string(n).c_str();
        GetTextDimensions(n < 128 ? font42 : font24, std::to_string(n).c_str(), &number_w, NULL);
        DrawText(n < 128 ? font42 : font24, g_xstart + (g_l + g_spacing) * j + (g_l - number_w) / 2,
            g_ystart + (g_l + g_spacing) * i + (g_l - (n < 128 ? 64 : 42)) / 2, (n > 4 && n < 128) || (n > 2048) ? color_light : color_dark, text);
    }
}