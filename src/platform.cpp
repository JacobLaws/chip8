//
// Created by LawsV on 8/18/2020.
//
#include "platform.hpp"

Graphics::Graphics(): window{nullptr}, renderer{nullptr}, texture{nullptr}, input{0}
{}

Graphics::Graphics(int displayWidth, int displayHeight, int textureWidth, int textureHeight)
{
    /*
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
    else
    {
        window = SDL_CreateWindow("Chip8 Emulator by Jacob Laws", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  displayWidth, displayHeight,SDL_WINDOW_SHOWN);
        if(window == nullptr)
            printf("Window could not be created. SDL Error: %s\n", SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(renderer == nullptr)
            printf("Window could not be created. SDL Error: %s\n", SDL_GetError());

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
        if(texture == nullptr)
            printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
    }
     */

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chip8 Emulator by Jacob Laws", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              displayWidth, displayHeight,SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 30, 139, 195, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);

}

Graphics::~Graphics()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Graphics::ProcessInput(Chip8 &chip8)
{
    while(SDL_PollEvent(&input) != 0)
    {
        if(input.type == SDL_QUIT)
            return true;
        else
        {
            switch(input.type)
            {
                case SDL_KEYDOWN:
                {
                    switch(input.key.keysym.sym)
                    {
                        case SDLK_1:
                            chip8.SetKeyOn(1);
                            break;

                        case SDLK_2:
                            chip8.SetKeyOn(2);
                            break;

                        case SDLK_3:
                            chip8.SetKeyOn(3);
                            break;

                        case SDLK_4:
                            chip8.SetKeyOn(0xC);
                            break;

                        case SDLK_q:
                            chip8.SetKeyOn(4);
                            break;

                        case SDLK_w:
                            chip8.SetKeyOn(5);
                            break;

                        case SDLK_e:
                            chip8.SetKeyOn(6);
                            break;

                        case SDLK_r:
                            chip8.SetKeyOn(0xD);
                            break;

                        case SDLK_a:
                            chip8.SetKeyOn(7);
                            break;

                        case SDLK_s:
                            chip8.SetKeyOn(8);
                            break;

                        case SDLK_d:
                            chip8.SetKeyOn(9);
                            break;

                        case SDLK_f:
                            chip8.SetKeyOn(0xE);
                            break;

                        case SDLK_z:
                            chip8.SetKeyOn(0xA);
                            break;

                        case SDLK_x:
                            chip8.SetKeyOn(0);
                            break;

                        case SDLK_c:
                            chip8.SetKeyOn(0xB);
                            break;

                        case SDLK_v:
                            chip8.SetKeyOn(0xF);
                            break;
                    }
                    break;
                }

                case SDL_KEYUP:
                {
                    switch (input.key.keysym.sym)
                    {
                        case SDLK_1:
                            chip8.SetKeyOff(1);
                            break;

                        case SDLK_2:
                            chip8.SetKeyOff(2);
                            break;

                        case SDLK_3:
                            chip8.SetKeyOff(3);
                            break;

                        case SDLK_4:
                            chip8.SetKeyOff(0xC);
                            break;

                        case SDLK_q:
                            chip8.SetKeyOff(4);
                            break;

                        case SDLK_w:
                            chip8.SetKeyOff(5);
                            break;

                        case SDLK_e:
                            chip8.SetKeyOff(6);
                            break;

                        case SDLK_r:
                            chip8.SetKeyOff(0xD);
                            break;

                        case SDLK_a:
                            chip8.SetKeyOff(7);
                            break;

                        case SDLK_s:
                            chip8.SetKeyOff(8);
                            break;

                        case SDLK_d:
                            chip8.SetKeyOff(9);
                            break;

                        case SDLK_f:
                            chip8.SetKeyOff(0xE);
                            break;

                        case SDLK_z:
                            chip8.SetKeyOff(0xA);
                            break;

                        case SDLK_x:
                            chip8.SetKeyOff(0);
                            break;

                        case SDLK_c:
                            chip8.SetKeyOff(0xB);
                            break;

                        case SDLK_v:
                            chip8.SetKeyOff(0xF);
                            break;
                    }
                    break;
                }
            }
        }
    }
    return false;
}

void Graphics::Update(const void* buffer, int pitch)
{
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}