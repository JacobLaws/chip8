//
// Created by LawsV on 8/18/2020.
//
#ifndef CHIP8_GRAPHICS_H
#define CHIP8_GRAPHICS_H

#include <SDL2/SDL.h>
#include "chip8.hpp"

class Graphics
{
    public:

        Graphics();
        Graphics(int, int, int, int);
        ~Graphics();

        bool ProcessInput(Chip8 &);
        void Update(const void* buffer, int pitch);

    private:
        SDL_Window* window{};
        SDL_Renderer* renderer{};
        SDL_Texture* texture{};
        SDL_Event input{};
};

#endif //CHIP8_GRAPHICS_H
