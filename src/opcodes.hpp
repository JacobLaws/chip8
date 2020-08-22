#ifndef CHIP8_OPCODES_H
#define CHIP8_OPCODES_H

#include "chip8.hpp"

class Opcodes: public Chip8
{
    public:
        Opcodes();
        ~Opcodes();

        void DecodeOpcode();

    protected:

        void (Opcodes::*opTable)();
        opTable 

        void op_00E0();
        void op_00EE();
        void op_1NNN();
        void op_2NNN();
        void op_3XNN();
        void op_4XNN();
        void op_5XY0();
        void op_6XNN();
        void op_7XNN();
        void op_8XY0();
        void op_8XY1();
        void op_8XY2();
        void op_8XY3();
        void op_8XY4();
        void op_8XY5();
        void op_8XY6();
        void op_8XY7();
        void op_8XYE();
        void op_9XY0();
        void op_ANNN();
        void op_BNNN();
        void op_CXNN();
        void op_DXYN();
        void op_EX9E();
        void op_EXA1();
        void op_FX07();
        void op_FX0A();
        void op_FX15();
        void op_FX18();
        void op_FX1E();
        void op_FX29();
        void op_FX33();
        void op_FX55();
        void op_FX65();
};

#endif //CHIP8_OPCODES_H
