#ifndef CHIP_8_CHIP8_H
#define CHIP_8_CHIP8_H

#include <cstdio>
#include <chrono>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32

class Chip8
{
    public:
        Chip8();
        ~Chip8();

        int LoadGame();

        void DecodeOpcode();
        void EmulateCycle();

        void Display();
        void Input();

    protected:
        void InitializeCPU();
        int InitializeGPU();

        void InitializeVAO();
        static void FrameBufferSizeCallback(GLFWwindow*, int width, int height);

        unsigned char memory[4026]; // 4K Memory
        unsigned char V[16];        // REGISTERS: V0 -> V15 (V16 is carryh

        unsigned short I;           // INDEX Register
        unsigned short pc;          // Program counter

        unsigned short delayTimer;  // Game event timer    -> 60 Hz
        unsigned short soundTimer;  // Sound effects timer -> 60 Hz

        unsigned short stack[16];   // 16 Level stack
        unsigned short sp;          // Stack pointer

        unsigned char keyPad[16];   // HEX based keypad  (0x0 -> 0xF)
        unsigned char gfx[64*32];   // RESOLUTION: 64 x 32 (2048 pixels)
        unsigned short opcode;      // Opcodes pointer (35 in total)

        std::default_random_engine randSeed;
        std::uniform_int_distribution<unsigned char> randByte;

        GLFWwindow* window;
};

#endif //CHIP_8_CHIP8_H
