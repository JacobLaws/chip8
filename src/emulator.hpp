#ifndef CHIP8_EMULATOR_H
#define CHIP8_EMULATOR_H

#include "chip8.hpp"
#include "opcodes.hpp"

class Emulator: public Opcodes
{
    public:
        Emulator();
        ~Emulator();

        int LoadROM();
        void EmulateCycle();

        void UpdateVideo();
        bool ProcessInput();

    private:
        SDL_Window*   window;
        SDL_Renderer* renderer;
        SDL_Texture*  texture;
        SDL_Event     input;
};

#endif //CHIP8_EMULATOR_H
