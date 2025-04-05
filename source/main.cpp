#include <switch.h>
#include <sys/stat.h>
#include "draw.hpp"
#include "game.hpp"

u8* g_framebuf;
u32 g_framebufWidth;

bool fileExists(std::string path)
{
    struct stat buffer;   
    return (stat (path.c_str(), &buffer) == 0);
}

int main()
{
    PadState pad;
    mkdir("sdmc:/switch", 777);
    mkdir("sdmc:/switch/2048", 777);
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    Framebuffer fb;
    framebufferCreate(&fb, nwindowGetDefault(), FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&fb);

    Game::init();
    if (fileExists("sdmc:/switch/2048/state"))
    {
        Game::loadState();
    }

    while(appletMainLoop())
    {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) break;

        Game::scanInput(kDown);
        Game::show(&fb);
    }

    Game::saveState();
    framebufferClose(&fb);
	return 0;
}