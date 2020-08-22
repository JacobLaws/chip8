#ifndef CHIP_8_CHIP8_H
#define CHIP_8_CHIP8_H

#include <cstdio>
#include <cstdint>
#include <chrono>
#include <random>

#include <SDL2/SDL.h>

class Chip8
{
    public:
        Chip8();
        ~Chip8();

        int LoadGame();

        void DecodeOpcode();
        void EmulateCycle();

        const void* GetGfx();
        void SetKeyOn(int keyIn);
        void SetKeyOff(int keyIn);

    //private:
        void Initialize();

        unsigned char memory[4026]; // 4K Memory
        unsigned char V[16];        // REGISTERS: V0 -> V15 (V16 is carryh

        unsigned short I;           // INDEX Register
        unsigned short pc;          // Program counter

        unsigned short delayTimer;  // Game event timer    -> 60 Hz
        unsigned short soundTimer;  // Sound effects timer -> 60 Hz

        unsigned short stack[16];   // 16 Level stack
        unsigned short sp;          // Stack pointer

        unsigned char keyPad[16];   // HEX based keypad  (0x0 -> 0xF)
        uint32_t gfx[64*32];   // RESOLUTION: 64 x 32 (2048 pixels)
        unsigned short opcode;      // Opcodes pointer (35 in total)

        std::default_random_engine randSeed;
        std::uniform_int_distribution<unsigned char> randByte;
};

#endif //CHIP_8_CHIP8_H
