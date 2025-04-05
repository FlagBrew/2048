#include "game.hpp"

const u32 g_l       = 96;
const u32 g_spacing = 16;
const u32 g_xstart  = ceil((1280 - g_l * 4 - g_spacing * 3) / 2);
const u32 g_ystart  = ceil((720 - g_l * 4 - g_spacing * 3) / 2);
color_t colorbg     = MakeColor(187, 173, 160, 255);

static std::array<Cell, 16> grid;
static u32 score;

u8 index(u8 row, u8 col)
{
    return row * 4 + col;
}

void Game::init(void)
{
    srand(time(NULL));
    score = 0;
    grid.fill(Cell(0));
    Grid::addNumber();
    Grid::addNumber();
}

void Grid::addNumber(void)
{
    u32 newNumber = (rand() % 10) == 9 ? 4 : 2;
    std::vector<u8> emptyIndices;

    for (u8 i = 0; i < 4; i++) {
        for (u8 j = 0; j < 4; j++) {
            u8 idx = index(i, j);
            if (grid[idx].value() == 0) {
                emptyIndices.push_back(idx);
            }
        }
    }

    if (emptyIndices.size() > 0) {
        grid[emptyIndices.at(rand() % emptyIndices.size())].value(newNumber);
    }
}

void Grid::slide(u8 row)
{
    // slide everything to right
    u8 i = index(row, 0);
    std::vector<Cell> values(grid.begin() + i, grid.begin() + i + 4);
    values.erase(std::remove_if(values.begin(), values.end(), [](Cell& cell) { return cell.value() == 0; }), values.end());
    values.insert(values.begin(), 4 - values.size(), 0);
    std::copy(values.begin(), values.end(), grid.begin() + i);
}

void Grid::combine(u8 row)
{
    for (u8 col = 3; col > 0; col--) {
        u8 curr = index(row, col);
        u8 next = index(row, col - 1);
        u32 a   = grid[curr].value();
        u32 b   = grid[next].value();
        if (a == b) {
            grid[curr].value(a + b);
            score += a + b;
            grid[next].value(0);
        }
    }
}

void Grid::operate(void)
{
    for (u8 row = 0; row < 4; row++) {
        slide(row);
        combine(row);
        slide(row);
    }
}

void Grid::transpose(void)
{
    std::array<Cell, 16> newgrid;
    for (u8 row = 0; row < 4; row++) {
        for (u8 col = 0; col < 4; col++) {
            newgrid[index(row, col)] = grid[index(col, row)];
        }
    }
    grid = newgrid;
}

void Grid::flip(void)
{
    for (u8 row = 0; row < 4; row++) {
        u8 start = index(row, 0);
        std::reverse(grid.begin() + start, grid.begin() + start + 4);
    }
}

void Game::scanInput(u64 kDown)
{
    if (kDown & HidNpadButton_B) {
        // restore game
        init();
    }
    else {
        std::array<Cell, 16> gridclone = grid;

        if (kDown & HidNpadButton_Up) {
            Grid::transpose();
            Grid::flip();
            Grid::operate();
            Grid::flip();
            Grid::transpose();
        }
        else if (kDown & HidNpadButton_Down) {
            Grid::transpose();
            Grid::operate();
            Grid::transpose();
        }
        else if (kDown & HidNpadButton_Left) {
            Grid::flip();
            Grid::operate();
            Grid::flip();
        }
        else if (kDown & HidNpadButton_Right) {
            Grid::operate();
        }

        for (u8 i = 0; i < 16; i++) {
            if (grid[i].value() != gridclone[i].value()) {
                Grid::addNumber();
                break;
            }
        }
    }
}

bool Game::gameOver(void)
{
    for (u8 row = 0; row < 4; row++) {
        for (u8 col = 0; col < 4; col++) {
            if (grid[index(row, col)].value() == 0) {
                return false;
            }
            if (row != 3 && grid[index(row, col)].value() == grid[index(row + 1, col)].value()) {
                return false;
            }
            if (col != 3 && grid[index(row, col)].value() == grid[index(row, col + 1)].value()) {
                return false;
            }
        }
    }
    return true;
}

bool Game::gameWon(void)
{
    for (u8 i = 0; i < 16; i++) {
        if (grid[i].value() >= 2048) {
            return true;
        }
    }
    return false;
}

void Game::show(Framebuffer* fb)
{
    u32 stride;
    g_framebuf      = (u8*)framebufferBegin(fb, &stride);
    g_framebufWidth = stride / sizeof(u32);

    // background
    memset(g_framebuf, 250, stride * FB_HEIGHT);

    // the matrix background
    rectangle(g_xstart - g_spacing, g_ystart - g_spacing, g_l * 4 + g_spacing * 5, g_l * 4 + g_spacing * 5, colorbg);

    u32 title_w, maxscore_w, score_w;
    char scorestr[16];
    sprintf(scorestr, "Score: %d", score);
    const char* title = gameWon() ? "Won!" : gameOver() ? "Lost :(" : "2048";
    GetTextDimensions(font42, title, &title_w, NULL);
    GetTextDimensions(font24, "Score: 3932156", &maxscore_w, NULL);
    GetTextDimensions(font24, scorestr, &score_w, NULL);

    // title
    DrawText(font42, g_xstart - g_spacing, 40, MakeColor(119, 110, 101, 255), title);

    // score background
    rectangle(g_xstart + g_l * 4 + g_spacing * 2 - maxscore_w, 42, maxscore_w + g_spacing * 2, 60, MakeColor(119, 110, 101, 255));
    // score
    DrawText(font24, g_xstart + g_l * 4 + g_spacing * 2 - maxscore_w + (maxscore_w + g_spacing * 2 - score_w) / 2, 52, MakeColor(205, 192, 180, 255),
        scorestr);

    // show cells
    for (u8 i = 0; i < 4; i++) {
        for (u8 j = 0; j < 4; j++) {
            grid[index(i, j)].show(i, j);
        }
    }

    if (gameOver()) {
        u32 lost_w;
        GetTextDimensions(font24, "Press B to restart.", &lost_w, NULL);
        DrawText(font24, ceil((1280 - lost_w) / 2), 640, MakeColor(119, 110, 101, 255), "Press B to restart.");

        PadState pad;
        while (appletMainLoop()) {
            padUpdate(&pad);
            u64 kDown = padGetButtonsDown(&pad);
            if (kDown & HidNpadButton_B)
                break;
            framebufferEnd(fb);
        }
        Game::init();
    }

    framebufferEnd(fb);
}

void Game::saveState(void)
{
    u8 buf[(16 + 1) * 4] = {0};
    for (u8 i = 0; i < 16; i++) {
        u32 value = grid[i].value();
        memcpy(buf + i * 4, &value, 4);
    }
    memcpy(buf + 16 * 4, &score, 4);
    FILE* out = fopen("sdmc:/switch/2048/state", "wb");
    fwrite(buf, (16 + 1) * 4, 1, out);
    fclose(out);
}

void Game::loadState(void)
{
    u8 buf[(16 + 1) * 4] = {0};
    FILE* fptr           = fopen("sdmc:/switch/2048/state", "rb");
    if (fptr == NULL) {
        fclose(fptr);
        return;
    }
    fread(buf, (16 + 1) * 4, 1, fptr);
    fclose(fptr);

    for (u8 i = 0; i < 16; i++) {
        u32 value;
        memcpy(&value, buf + i * 4, 4);
        grid[i].value(value);
    }
    memcpy(&score, buf + 16 * 4, 4);
}