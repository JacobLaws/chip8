#include "opcodes.hpp"


Opcodes::Opcodes(): Chip8::Chip8()
{}

Opcodes::~Opcodes()
{}

void Opcodes::DecodeOpcode()
{


}

// 0x00E0: Clears the screen (CLS)
void Opcodes::op_00E0()
{
    memset(gfx, 0, sizeof(gfx));
}

// 0x00EE: Returns from subroutine (RET)
void Opcodes::op_00EE()
{
    --sp;
    pc = stack[sp];
}

// 0x1NNN: Jumps to location NNN (JP addr)
void Opcodes::op_1NNN()
{
    pc = opcode & 0xFFF0u;
}

// 0x2NNN: Calls subroutine at NNN (CALL addr)
void Opcodes::op_2NNN()
{
    stack[sp] = pc;
    ++sp;
    pc = opcode & 0x0FFFu;
}

// 0x3XNN: Skips next instruction if Vx = NN (SE Vx, byte)
void Opcodes::op_3XNN()
{
    if( (V[(opcode & 0x0F00u) >> 8u]) == (opcode & 0x00FFu) )
        pc += 2;
}

// 0x4XNN: Skips next instruction if Vx != NN (SE Vx, byte)
void Opcodes::op_4XNN()
{
    if( (V[(opcode & 0x0F00u) >> 8u]) != (opcode & 0x00FFu) )
        pc += 2;
}

// 0x5XY0: Skips next instruction if Vx = Vy (SE Vx, Vy)
void Opcodes::op_5XY0()
{
    if( (V[(opcode & 0x0F00u) >> 8u]) == (V[(opcode & 0x00F0u) >> 4u]) )
        pc += 2;
}

// 0x6XNN: Sets Vx = NN (LD Vx, byte)
void Opcodes::op_6XNN()
{
    V[(opcode & 0x0F00u) >> 8u] = opcode & 0x00FFu;
}

// 0x7XNN: Sets Vx = Vx + NN (ADD Vx, byte)
void Opcodes::op_7XNN()
{
    V[(opcode & 0x0F00u) >> 8u] += (opcode & 0x00FFu);
}

// 0x8XY0: Sets Vx = Vy (LD Vx, byte)
void Opcodes::op_8XY0()
{
    V[(opcode & 0x0F00u) >> 8u] = V[(opcode & 0x00F0u) >> 4u];
}

// 0x8XY1: Sets Vx = Vx OR Vy (OR Vx, Vy)
void Opcodes::op_8XY1()
{
    V[(opcode & 0x0F00u) >> 8u] |= V[(opcode & 0x00F0u) >> 4u];
}

// 0x8XY2: Sets Vx = Vx AND Vy (AND Vx, Vy)
void Opcodes::op_8XY2()
{
    V[(opcode & 0x0F00u) >> 8u] &= V[(opcode & 0x00F0u) >> 4u];
}

// 0x8XY3: Sets Vx = Vx XOR Vy (XOR Vx, Vy)
void Opcodes::op_8XY3()
{
    V[(opcode & 0x0F00u) >> 8u] ^= V[(opcode & 0x00F0u) >> 4u];
}

// 0x8XY4: Sets Vx = Vx + Vy (ADD Vx, Vy)
void Opcodes::op_8XY4()
{
    if ( (V[(opcode & 0x0F00u) >> 8u] + V[(opcode & 0x00F0u) >> 4u]) > 255u )
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00u) >> 8u] = (V[(opcode & 0x0F00u) >> 8u] + V[(opcode & 0x00F0u) >> 4u]);
    V[(opcode & 0x0F00u) >> 8u] &= 0x00FFu;
}

// 0x8XY5: Sets Vx = Vx - Vy (SUB Vx, Vy)
void Opcodes::op_8XY5()
{
    if( V[(opcode & 0x0F00u) >> 8u] > V[(opcode & 0x00F0u) >> 4u] )
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00u) >> 8u] -= V[(opcode & 0x00F0u) >> 4u];
}

// 0x8XY6: Sets Vx = Vx SHR Vy (SHR Vx {, Vy})
void Opcodes::op_8XY6()
{
    if( (V[(opcode &0x0F00u) >> 8u] & 0x01u) == 0x01u )
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00u) >> 8u] /= 2;
}

// 0x8XY7: Sets Vx = Vy - Vx (SUB Vx, Vy)
void Opcodes::op_8XY7()
{
    if( V[(opcode & 0x0F00u) >> 8u] < V[(opcode & 0x00F0u) >> 4u])
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00u) >> 8u] = V[(opcode & 0x00F0u) >> 4u] - V[(opcode & 0x0F00u) >> 8u];
}

// 0x8XYE: Sets Vx = Vx SHL 1 (SHL Vx, 1)
void Opcodes::op_8XYE()
{
    if( (V[(opcode &0x0F00u) >> 8u] & 0x80u) >> 7u == 0x01u )
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[(opcode & 0x0F00u) >> 8u] *= 2;
}

// 0x9XY0: Skips next instruction if Vx != Vy (SNE Vx, Vy)
void Opcodes::op_9XY0()
{
    if( V[(opcode & 0x0F00u) >> 8u] != V[(opcode & 0x00F0u) >> 4u] )
        pc += 2;
}

// 0xANNN: Sets I = NNN (LD I, addr)
void Opcodes::op_ANNN()
{
    I = opcode & 0x0FFFu;
}

// 0xBNNN: Jumps to location NNN + V0 (JP V0, addr)
void Opcodes::op_BNNN()
{
    pc = (opcode & 0x0FFFu) + V[0];
}

// 0xCXNN: Sets Vx = random byte AND NN (RND Vx, byte)
void Opcodes::op_CXNN()
{
    V[(opcode & 0x0F00u) >> 8u] = randByte(randSeed) & (opcode & 0x00FFu);
}

// 0xDXYN: Displays n-byte sprite starting at memory location I at (Vx, Vy). (DRW Vx, Vy, nibble)
void Opcodes::op_DXYN()
{
    unsigned char Vx = V[(opcode & 0x0F00u) >> 8u];
    unsigned char Vy = V[(opcode & 0x00F0u) >> 4u];
    unsigned char height = opcode & 0x000Fu;

    unsigned char xPos = Vx % 64;
    unsigned char yPos = Vy % 32;

    V[0xF] = 0;

    for(unsigned int row = 0; row < height; ++row)
    {
        uint32_t rowPixels = memory[I + row];

        for (unsigned int col = 0; col < 8; ++col) {
            if (rowPixels & (0x80u >> col)) {
                if (gfx[((xPos + col) + ((yPos + row) * 64)) % 2048] == 0xFFFFFFFFu)
                    V[0xF] |= 1u;

                gfx[((xPos + col) + ((yPos + row) * 64)) % 2048] ^= 0xFFFFFFFFu;
            }
        }
    }
}

// 0xEX9E: Skips next instruction if the key containing Vx IS pressed (SKP Vx)
void Opcodes::op_EX9E()
{
    if( keyPad[V[(opcode & 0x0F00u) >> 8u]] != 0 )
        pc += 2;
}

// 0xEXA1: Skips next instruction if the key containing Vx is NOT pressed (SKP Vx)
void Opcodes::op_EXA1()
{
    if(keyPad[V[(opcode & 0x0F00u) >> 8u]] == 0)
        pc += 2;
}

// 0xFX07: Sets Vx = delay timer value (Ld Vx, DT)
void Opcodes::op_FX07()
{
    V[(opcode & 0x0F00u) >> 8u] = delayTimer;
}

// 0xFX0A: Waits for a key press, then stores the value of the key into Vx (Ld Vx, K)
void Opcodes::op_FX0A()
{
    unsigned char x = (opcode & 0x0F00u) >> 8u;

    if(keyPad[0])
        V[x] = 0;
    else if(keyPad[1])
        V[x] = 1;
    else if(keyPad[2])
        V[x] = 2;
    else if(keyPad[3])
        V[x] = 3;
    else if(keyPad[4])
        V[x] = 4;
    else if(keyPad[5])
        V[x] = 5;
    else if(keyPad[6])
        V[x] = 6;
    else if(keyPad[7])
        V[x] = 7;
    else if(keyPad[8])
        V[x] = 8;
    else if(keyPad[9])
        V[x] = 9;
    else if(keyPad[0xA])
        V[x] = 0xA;
    else if(keyPad[0xB])
        V[x] = 0xB;
    else if(keyPad[0xC])
        V[x] = 0xC;
    else if(keyPad[0xD])
        V[x] = 0xD;
    else if(keyPad[0xE])
        V[x] = 0xE;
    else if(keyPad[0xF])
        V[x] = 0xF;
    else
        pc -= 2;
}

// 0xFX15: Sets delayTimer == Vx (LD displayTimer, Vx)
void Opcodes::op_FX15()
{
    delayTimer = V[(opcode & 0x0F00u) >> 8u];
}

// 0xFX18: Sets soundTimer == Vx (LD soundTimer, Vx)
void Opcodes::op_FX18()
{
    soundTimer = V[(opcode & 0x0F00u) >> 8u];
}

// 0xFX1E: Sets I = I + Vx (ADD I, Vx)
void Opcodes::op_FX1E()
{
    I += V[(opcode & 0x0F00u) >> 8u];
}

// 0xFX29: Sets I = location of Vx's digit sprite (LD F, Vx)
void Opcodes::op_FX29()
{
    I = memory[0x50u + (V[(opcode & 0x0F00u) >> 8u] * 5)];
}

// 0xFX33: Stores BCD (binary coded decimal) representation of Vx in I, I+1, and I+2 (LD B, Vx)
void Opcodes::op_FX33()
{
    unsigned char Vx = V[(opcode & 0x0F00u) >> 8u];

    memory[I]     = Vx / 100;
    memory[I + 1] = (Vx / 10) % 10;
    memory[I + 2] = Vx % 10;
}

// 0xFX55: Stores registers V0 -> Vx in memory starting at location I (index register)
void Opcodes::op_FX55()
{
    for(unsigned int j = 0; j <= ((opcode & 0x0F00u) >> 8u); ++j)
        memory[I + j] = V[j];
}

// 0xFX65: Reads registers V0 -> Vx from memory starting at location I (index register)
void Opcodes::op_FX65()
{
    for(unsigned int j = 0; j <= ((opcode & 0x0F00u) >> 8u); ++j)
        V[j] = memory[I + j];
}