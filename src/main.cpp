#include "emulator.hpp"

int main(int argv, char** args)
{
    Emulator chip8;

    if(chip8.LoadROM() != 0)
    {
        std::fprintf(stderr, "Failed to load the ROM\n");
        return -4;
    }

    auto prevCycleTime = std::chrono::high_resolution_clock::now();
    float cycleDelay   = 1.0;

    bool quit = false;

    while(!quit)
    {
        quit = chip8.ProcessInput();

        auto currentCycleTime = std::chrono::high_resolution_clock::now();
        float delay = std::chrono::duration<float, std::chrono::milliseconds::period>(currentCycleTime - prevCycleTime).count();

        if(delay > cycleDelay)
        {
            prevCycleTime = currentCycleTime;

            chip8.EmulateCycle();

            chip8.UpdateVideo();
        }
    }
    return 0;
}

