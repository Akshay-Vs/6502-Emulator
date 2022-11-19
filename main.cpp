#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;
using Word = unsigned short;
using U32 = unsigned int;

class Mem
{
public:
    // read 1 byte
    Byte operator[](U32 address) const
    {
        return Data[address];
    }

    // write 1 byte
    Byte& operator[](U32 address)
    {
        return Data[address];
    }
    static constexpr U32 MEM_SIZE = 1024 * 64;
    Byte Data[MEM_SIZE];

    void Initialize()
    {
        for (U32 i = 0; i < MEM_SIZE; ++i)
        {
            Data[i] = 0;
        }
    }
};

class CPU
{

    Word PC; // program counter
    Word SP; // stack pointer

    Byte A, X, Y; // registers

    Byte C : 1; // carry flag
    Byte Z : 1; // zero flag
    Byte I : 1; // interrupt disable
    Byte D : 1; // decimal mode
    Byte B : 1; // break command
    Byte V : 1; // overflow flag
    Byte N : 1; // negative flag

    Byte FetchByte(U32 &Cycles, Mem& memory)
    {
        Byte Data = memory.Data[PC];
        PC++;
        Cycles--;
        return Data;
    }

public:
    // opcodes
    static constexpr Byte INS_LDA_IM = 0xA9;
    void Reset(Mem& memory)
    {
        PC = 0xFFFC;
        SP = 0x0100;
        A = X = Y = 0;
        C = Z = I = D = B = V = N = 0;
        memory.Initialize();
    }

    void Execute(U32 Cycles, Mem& memory)
    {
        while (Cycles > 0)
        {
            Byte instruction = FetchByte(Cycles, memory);
            switch (instruction)
            {
            case INS_LDA_IM:
            {
                Byte value = FetchByte(Cycles, memory);
                A = value;
                Z = (A == 0);
                N = (A & 0b10000000) > 0;
            }
            break;
            default:
            {
                std::cout << "Unknown instruction: " << instruction << std::endl;
            }
            break;
            }
        }
    }
};

int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);

    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x42;

    cpu.Execute(2, mem);
    return 0;
}