//
// Created by kayla on 10/12/23.
//

#ifndef FANTASY8_CHIP8_CONTEXT_HPP
#define FANTASY8_CHIP8_CONTEXT_HPP

#include <cstdint>
#include <bitset>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

namespace chip8 {

	class Chip8_Context {
		std::uint16_t PC;       // Program Counter, stores the currently executing address
		std::uint8_t SP;        // Stack Pointer, points to the topmost level of the stack

		// general-purpose 8-bit registers
		std::uint8_t V0;
		std::uint8_t V1;
		std::uint8_t V2;
		std::uint8_t V3;
		std::uint8_t V4;
		std::uint8_t V5;
		std::uint8_t V6;
		std::uint8_t V7;
		std::uint8_t V8;
		std::uint8_t V9;
		std::uint8_t VA;
		std::uint8_t VB;
		std::uint8_t VC;
		std::uint8_t VD;
		std::uint8_t VE;
		std::uint8_t VF;
		std::uint16_t I;        // special-purpose 16-bit register
		std::uint8_t DT;        // Delay Timer register
		std::uint8_t ST;        // Sound Timer register

		std::uint8_t memory[4096];  // 4096 bytes of memory
		std::uint16_t stack[16];    // 16 bit stack with 16 values

		std::bitset<DISPLAY_WIDTH*DISPLAY_HEIGHT> display_buffer;
	};

}

#endif //FANTASY8_CHIP8_CONTEXT_HPP
