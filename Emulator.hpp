#pragma once
#include <cstdint>
#include <fstream>
#include <chrono>
#include <random>

enum Registers {
    V0,
    V1,
    V2,
    V3,
    V4,
    V5,
    V6,
    V7,
    V8,
    V9,
    VA,
    VB,
    VC,
    VD,
    VE,
    VF // Flag that holds the information about the result of the operation.
};

const unsigned int FONTSET_SIZE = 80;

uint8_t fontset[FONTSET_SIZE] =
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

class Emulator {
    private:
        uint8_t registers[16]; // For executing instructions
        uint8_t memory[4096]; // Storing data and result of execution
        uint16_t index_register; // Stores memory addresses for use in operations
        uint16_t program_counter; // Stores memory address for the next instruction to execute
        uint16_t stack[16]; // 16 level stack to know which instruction should run first
        uint8_t stack_pointer; // Keeps track of where in memory the cpu is executing instructions
        uint8_t delay_timer;
        uint8_t sound_timer;
        uint8_t keypad[16]; // Detection of keypresses
        uint32_t video_data[64 * 32]; // For video data, displaying sprites...etc
        uint16_t opcode;
        std::default_random_engine randGen;
	    std::uniform_int_distribution<uint8_t> randByte;

        void    LoadROM(char const *filename);
        void    cls(); // Clear Display OP_00E0
        void    ret(); // Return from subroutine OP_00EE
        void    jp(); // Jump to location nnn OP_1nnn
        void    call(); // Call subroutine at nnn OP_2nnn
        void    se(); // Skips the next instruction if Register Vx == kk OP_3xkk
        void    seNOT(); // Skips the next instruction if Register Vx != kk OP_4xkk
        void    seXY(); // Skips the next instruction if Vx == Vy OP_5xy0
        void    ld(); // Sets the value of Register Vx to kk OP_6xkk
        void    add(); // Adds to the value inside Register Vx OP_7xkk
        void    ldSetRegister(); // Sets the value of Register Vx to Vy's OP_8xy0
        void    and(); // Sets the value of Vx to (Vx & Vy) OP_8xy2 
        void    xor(); // Sets the value of Vx to (Vx ^ Vy) OP_8xy3
};