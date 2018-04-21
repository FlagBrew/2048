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
    mkdir("sdmc:/switch", 777);
    mkdir("sdmc:/switch/2048", 777);
    gfxInitDefault();

    Game::init();
    if (fileExists("sdmc:/switch/2048/state"))
    {
        Game::loadState();
    }

    while(appletMainLoop() && !(hidKeysDown(CONTROLLER_P1_AUTO) & KEY_PLUS))
    {
        Game::scanInput();
        Game::show();
    }

    Game::saveState();
    gfxExit();
	return 0;
}