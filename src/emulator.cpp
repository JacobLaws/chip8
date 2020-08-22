#include "emulator.hpp"

Emulator::Emulator()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chip8 Emulator by Jacob Laws", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              config::DISPLAY_WIDTH, config::DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING,
                                config::VIDEO_WIDTH, config::VIDEO_HEIGHT);
}

/*
Emulator::Emulator(int displayWidth, int displayHeight, int textureWidth, int textureHeight)
{
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

    SDL_SetRenderDrawColor(renderer, 30, 139, 195, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
 */

Emulator::~Emulator()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int Emulator::LoadROM()
{
    FILE* rom;
    unsigned short bufferSize;
    unsigned char* buffer;
    size_t readLen;

    rom = fopen(R"(C:\Users\LawsV\Documents\Google Drive\Hobbies\Programming\C++\Emulators\chip8\roms (David Winter)\BRIX)", "rb");
    if(rom == nullptr)
    {
        std::fprintf(stderr, "Unable to open find/open ROM");
        return -1;
    }

    // Obtain the buffer/file size
    fseek(rom, 0, SEEK_END);
    bufferSize = ftell(rom);
    rewind(rom);

    buffer = new unsigned char[bufferSize];
    if(buffer == nullptr)
    {
        fputs("Error: Memory error", stderr);
        return -2;
    }

    readLen = fread(buffer, 1, bufferSize, rom);
    if(readLen != bufferSize)
    {
        fputs("Error: File read error", stderr);
        return -3;
    }

    for(int i = 0; i < bufferSize; ++i)
    {
        memory[i + 512] = buffer[i];
    }

    fclose(rom);
    delete [] buffer;

    return 0;
}

void Emulator::EmulateCycle()
{
    // Merge the two bytes of memory to construct the current 2 byte opcode
    opcode = memory[pc] << 8u | memory[pc + 1];

    // Increment the program counter by 2 bytes
    pc += 2;

    // Decode and execute the current opcode
    Opcodes::DecodeOpcode();

    // Decrement counters if activated
    if(delayTimer > 0)
        --delayTimer;

    if(soundTimer > 0)
    {
        if(soundTimer == 1)
            printf("bzzp!\n");
        --soundTimer;
    }
}

void Emulator::UpdateVideo()
{
    SDL_UpdateTexture(texture, nullptr, gfx, config::DISPLAY_PITCH);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Emulator::ProcessInput()
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
                            keyPad[1] = 1;
                            break;

                        case SDLK_2:
                            keyPad[2] = 1;
                            break;

                        case SDLK_3:
                            keyPad[3] = 1;
                            break;

                        case SDLK_4:
                            keyPad[0xC] = 1;
                            break;

                        case SDLK_q:
                            keyPad[4] = 1;
                            break;

                        case SDLK_w:
                            keyPad[5] = 1;
                            break;

                        case SDLK_e:
                            keyPad[6] = 1;
                            break;

                        case SDLK_r:
                            keyPad[0xD] = 1;
                            break;

                        case SDLK_a:
                            keyPad[7] = 1;
                            break;

                        case SDLK_s:
                            keyPad[8] = 1;
                            break;

                        case SDLK_d:
                            keyPad[9] = 1;
                            break;

                        case SDLK_f:
                            keyPad[0xE] = 1;
                            break;

                        case SDLK_z:
                            keyPad[0xA] = 1;
                            break;

                        case SDLK_x:
                            keyPad[0] = 1;
                            break;

                        case SDLK_c:
                            keyPad[0xB] = 1;
                            break;

                        case SDLK_v:
                            keyPad[0xF] = 1;
                            break;
                    }
                    break;
                }

                case SDL_KEYUP:
                {
                    switch (input.key.keysym.sym)
                    {
                        case SDLK_1:
                            keyPad[1] = 0;
                            break;

                        case SDLK_2:
                            keyPad[2] = 0;
                            break;

                        case SDLK_3:
                            keyPad[3] = 0;
                            break;

                        case SDLK_4:
                            keyPad[0xC] = 0;
                            break;

                        case SDLK_q:
                            keyPad[4] = 0;
                            break;

                        case SDLK_w:
                            keyPad[5] = 0;
                            break;

                        case SDLK_e:
                            keyPad[6] = 0;
                            break;

                        case SDLK_r:
                            keyPad[0xD] = 0;
                            break;

                        case SDLK_a:
                            keyPad[7] = 0;
                            break;

                        case SDLK_s:
                            keyPad[8] = 0;
                            break;

                        case SDLK_d:
                            keyPad[9] = 0;
                            break;

                        case SDLK_f:
                            keyPad[0xE] = 0;
                            break;

                        case SDLK_z:
                            keyPad[0xA] = 0;
                            break;

                        case SDLK_x:
                            keyPad[0] = 0;
                            break;

                        case SDLK_c:
                            keyPad[0xB] = 0;
                            break;

                        case SDLK_v:
                            keyPad[0xF] = 0;
                            break;
                    }
                    break;
                }
            }
        }
    }
    return false;
}