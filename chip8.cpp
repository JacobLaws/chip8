#include "chip8.h"
#include <cstdio>


Chip8::Chip8(): memory{0}, V{0}, I(0), pc(0), delayTimer(0), soundTimer(0), stack{0}, sp(0),
                keyPad{0}, gfx{0}, opcode(0), randSeed(std::chrono::system_clock::now().time_since_epoch().count())
{
    randByte = std::uniform_int_distribution<unsigned char>(0, 255u);

    this->Initialize();
}

Chip8::~Chip8()
{ }

void Chip8::Initialize()
{
    pc     = 0x200u; //Program counter starts at 0x200
    opcode = 0;      // Reset current opcode
    I      = 0;      // Reset index register
    sp     = 0;      // Reset stack pointer

    // Clear display
    // Clear stack
    // Clear registers V0 -> VF
    // Clear memory

    unsigned char fontset[80]   // Each number/character is 4 x 5 pixels
            {
                    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
            };

    // Load fontset
    for(int i = 0; i < 80; ++i)
    {
        memory[i] = fontset[i];
    }

    delayTimer = 0;
    soundTimer = 0;
}


int Chip8::LoadGame()
{
    FILE* rom;
    unsigned short bufferSize;
    unsigned char* buffer;
    size_t readLen;

    rom = fopen(R"(C:\Users\LawsV\Documents\Google Drive\Hobbies\Programming\C++\Emulators\chip-8\roms (David Winter)\PONG)", "rb");
    if(rom == nullptr)
    {
        fputs("Error: rom could not be read", stderr);
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

/*
     * Fetch Opcode
     * Decode Opcode
     * Execute Opcode
     * Update Timers
*/
void Chip8::DecodeOpcode()
{
    switch(opcode & 0xF000u)
    {
        case 0x0000:
            switch(opcode & 0x000Fu)
            {
                // ? VIDEO OPCODE
                case 0x0000: // 0x00E0: Clears the screen (CLS)
                    glClearColor(0.0, 0.0, 0.0, 0.0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    break;

                case 0x000E: // 0x00EE: Returns from subroutine (RET)
                    pc = stack[sp];
                    --sp;
                    break;
            }
            break;

        case 0x1000: // 0x1nnn: Jumps to location nnn (JP addr)
            pc = opcode & 0x0FFFu;
            break;

        case 0x2000: // 0x2nnn: Calls subroutine at nnn (CALL addr)
            ++sp;
            stack[sp] = pc;
            pc = opcode & 0x0FFFu;
            break;

        case 0x3000: // 0x3xkk: Skips next instruction if Vx = kk (SE Vx, byte)
            if( (V[(opcode & 0x0F00u) >> 8u]) == (opcode & 0x00FFu) )
                pc += 2;
            break;

        case 0x4000: //0x4xkk: Skips next instruction if Vx != kk (SE Vx, byte)
            if( (V[(opcode & 0x0F00u) >> 8u]) != (opcode & 0x00FFu) )
                pc += 2;
            break;

        case 0x5000: // 0x5xy0: Skips next instruction if Vx = Vy (SE Vx, Vy)
            if( (V[(opcode & 0x0F00u) >> 8u]) == (V[(opcode & 0x00F0u) >> 4u]) )
                pc +=2;
            break;

        case 0x6000: // 0x6xkk: Sets Vx = kk (LD Vx, byte)
            V[(opcode & 0x0F00u) >> 8u] = opcode & 0x00FFu;
            break;

        case 0x7000: // 0x7xkk: Sets Vx = Vx + kk (ADD Vx, byte)
            V[(opcode & 0x0F00u) >> 8u] += (opcode & 0x00FFu);
            break;

        case 0x8000:
            switch(opcode & 0x000Fu)
            {
                case 0x0000: // 0x8xy0: Sets Vx = Vy (LD Vx, byte)
                    V[(opcode & 0x0F00u) >> 8u] = V[(opcode & 0x00F0u) >> 4u];
                    break;

                case 0x0001: //0x8xy1: Sets Vx = Vx OR Vy (OR Vx, Vy)
                    V[(opcode & 0x0F00u) >> 8u] |= V[(opcode & 0x00F0u) >> 4u];
                    break;

                case 0x0002: //0x8xy2: Sets Vx = Vx AND Vy (AND Vx, Vy)
                    V[(opcode & 0x0F00u) >> 8u] &= V[(opcode & 0x00F0u) >> 4u];
                    break;

                case 0x0003: //0x8xy3: Sets Vx = Vx XOR Vy (XOR Vx, Vy)
                    V[(opcode & 0x0F00u) >> 8u] ^= V[(opcode & 0x00F0u) >> 4u];
                    break;

                case 0x0004: //0x8xy4: Sets Vx = Vx + Vy (ADD Vx, Vy)

                    if( (V[(opcode & 0x0F00u) >> 8u] + V[(opcode & 0x00F0u) >> 4u]) > 255u )
                        V[15] = 1;
                    else
                        V[15] = 0;

                    V[(opcode & 0x0F00u) >> 8u] = V[(opcode & 0x0F00u) >> 8u] + V[(opcode & 0x00F0u) >> 4u] & 0x00FFu;
                    break;

                case 0x0005: //0x8xy5: Sets Vx = Vx - Vy (SUB Vx, Vy)
                    if( V[(opcode & 0x0F00u) >> 8u] > V[(opcode & 0x00F0u) >> 4u])
                        V[15] = 1;
                    else
                        V[15] = 0;

                    V[(opcode & 0x0F00u) >> 8u] -= V[(opcode & 0x00F0u) >> 4u] & 0x00FFu;
                    break;

                case 0x0006: //0x8xy6: Sets Vx = Vx SHR Vy (SHR Vx {, Vy})
                    if( (V[(opcode &0x0F00u) >> 8u] & 0x1u) == 0x1u )
                        V[15] = 1;
                    else
                        V[15] = 0;

                    V[(opcode & 0x0F00u) >> 8u] /= 2;
                    break;

                case 0x0007: //0x8xy7: Sets Vx = Vx SUBN Vy (SUBN Vx, Vy)
                    if( V[(opcode & 0x0F00u) >> 8u] < V[(opcode & 0x00F0u) >> 4u])
                        V[15] = 1;
                    else
                        V[15] = 0;

                    V[(opcode & 0x0F00u) >> 8u] = V[(opcode & 0x00F0u) >> 4u] - V[(opcode & 0x0F00u) >> 8u];
                    break;

                case 0x000E: //0x8xyE: Sets Vx = Vx SHL 1 (SHL Vx, 1)
                    if( (V[(opcode &0x0F00u) >> 8u] & 0x80u) >> 7u == 0x1u )
                        V[15] = 1;
                    else
                        V[15] = 0;

                    V[(opcode & 0x0F00u) >> 8u] *= 2;
                    break;
            }
            break;

        case 0x9000: // 0x9xy0: Skips next instruction if Vx != Vy (SNE Vx, Vy)
            if( V[(opcode & 0x0F00u) >> 8u] != V[(opcode & 0x00F0u) >> 4u] )
                pc += 2;
            break;

        case 0xA000: // 0xAnnn: Sets I = nnn (LD I, addr)
            I = opcode & 0x0FFFu;
            break;

        case 0xB000: // 0xBnnn: Jumps to location nnn + V0 (JP V0, addr)
            pc = (opcode & 0x0FFFu) + V[0];
            break;

        case 0xC000: // 0xCxkk: Sets Vx = random byte AND kk (RND Vx, byte)
            V[(opcode & 0x0F00u) >> 8u] = randByte(randSeed) & (opcode & 0x00FFu);
            break;

        // ? VIDEO OPCODE
        case 0xD000: // 0xDxyn: Displays n-byte sprite starting at memory location I at (Vx, Vy). (DRW Vx, Vy, nibble)

            break;

        case 0xE000:

            switch(opcode & 0x00F0)
            {
                case 0x0090: // 0xEx9E: Skips next instruction if the key containing Vx IS pressed (SKP Vx)

                break;

                case 0x00A0: // 0xExA1: Skips next instruction if the key containing is NOT pressed (SKNP Vx)

                break;
            }

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007: // 0xFx07: Sets Vx = delay timer value (Ld Vx, DT)
                    V[(opcode & 0x0F00u) >> 8u] = delayTimer;
                    break;

                case 0x000A: // Waits for a key press, then stores the value of the key into Vx (Ld Vx, K)

                    break;

                case 0x0015: // Sets delayTimer == Vx (LD displayTimer, Vx)
                    delayTimer = V[(opcode & 0x0F00u) >> 8u];
                    break;

                case 0x0018: // Sets soundTimer == Vx (LD soundTimer, Vx)
                    soundTimer = V[(opcode & 0x0F00u) >> 8u];
                    break;

                case 0x001E: // Sets I = I + Vx (ADD I, Vx)
                    I += V[(opcode & 0x0F00u) >> 8u];
                    break;

                case 0x0029: // Sets I = location of Vx's digit sprite (LD F, Vx)

                    break;

                case 0x0033: // Stores BCD (binary coded decimal) representation of Vx in I, I+1, and I+2 (LD B, Vx)

                    break;

                case 0x0055: // Stores registers V0 -> Vx in memory starting at location I (index register)
                    for(int j = 0; j <= (opcode & 0x0F00u); ++j)
                        memory[I + j] = V[j];
                    break;

                case 0x0065: // Reads registers V0 -> Vx from memory starting at location I (index register)
                    for(int j = 0; j <= (opcode & 0x0F00u); ++j)
                        V[j] = memory[I + j];
                    break;
            }
    }
}


void Chip8::EmulateCycle()
{
    // Merge the two bytes of memory to construct the current 2 byte opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    this->DecodeOpcode();

    if(delayTimer > 0)
        --delayTimer;

    if(soundTimer > 0)
    {
        if(soundTimer == 1)
            printf("bzzp!\n");
        --soundTimer;
    }
}

void Chip8::Display()
{
    this->EmulateCycle();


}

/*
void openFile()
{
    FILE* fp = fopen("/roms (David Winter)/pong", "rb");

    if(!fp)
    {
        perror("File not found");
        return FILE_READ_FAILURE;
    }

    while((c = fgetc(fp)) != EOF)
    {

    }

    if(ferror(fp))
        puts("I/O error when reading");
    else if(feof(fp))
        puts("End of file reached successfully");

    fclose(fp);
}
*/
