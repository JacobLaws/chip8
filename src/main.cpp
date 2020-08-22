#include "chip8.hpp"
#include "platform.hpp"

int main(int argv, char** args)
{
    static const int windowWidth  = 64;
    static const int windowHeight = 32;
    static const int displayScale = 20;
    int displayWidth  = windowWidth  * displayScale;
    int displayHeight = windowHeight * displayScale;
    int gfxPitch      = windowWidth * sizeof(uint32_t);

    bool quit = false;

    Chip8 chip8;
    Graphics plat(displayWidth, displayHeight, windowWidth, windowHeight);

    if(chip8.LoadGame() != 0)
    {
        std::fprintf(stderr, "Failed to load the ROM\n");
        return -4;
    }

    auto prevCycleTime = std::chrono::high_resolution_clock::now();
    int cycleDelay     = 1;

    while(!quit)
    {
        quit = plat.ProcessInput(chip8);

        auto currentCycleTime = std::chrono::high_resolution_clock::now();
        float delay = std::chrono::duration<float, std::chrono::milliseconds::period>(currentCycleTime - prevCycleTime).count();

        if(delay > cycleDelay)
        {
            prevCycleTime = currentCycleTime;

            chip8.EmulateCycle();

            plat.Update(chip8.gfx, gfxPitch);
        }
    }
    return 0;
}

