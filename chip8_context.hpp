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

	class Context {
		private:
			bool ldflag = false;

			std::uint16_t PC = 0x200;   // Program Counter, stores the currently executing address
			// Most Chip-8 programs start here
			// ETI-600 programs start at 0x600
			// The lower locations are reserved for the interpreter

			std::uint8_t SP = 0;        // Stack Pointer, points to the topmost level of the stack

			// general-purpose 8-bit registers V(0-F)
			std::uint8_t V[16];

			std::uint16_t I;        // special-purpose 16-bit register

			std::uint8_t DT;        // Delay Timer register
			std::uint8_t ST;        // Sound Timer register

			std::uint8_t memory[4096] = {
					0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0 sprite
					0x20, 0x60, 0x20, 0x20, 0x70,   // 1 sprite
					0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2 sprite
					0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3 sprite
					0x90, 0x90, 0xF0, 0x10, 0x10,   // 4 sprite
					0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5 sprite
					0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6 sprite
					0xF0, 0x10, 0x20, 0x40, 0x40,   // 7 sprite
					0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8 sprite
					0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9 sprite
					0xF0, 0x90, 0xF0, 0x90, 0x90,   // A sprite
					0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B sprite
					0xF0, 0x80, 0x80, 0x80, 0xF0,   // C sprite
					0xE0, 0x90, 0x90, 0x90, 0xE0,   // D sprite
					0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E sprite
					0xF0, 0x80, 0xF0, 0x80, 0x80    // F sprite
			};  // 4096 bytes of memory

			std::uint16_t stack[16];    // 16 bit stack with 16 values

			// Peripherals
			bool keyboard[16] = {};
			std::bitset<DISPLAY_WIDTH*DISPLAY_HEIGHT> display_buffer;
		public:
			// ROM loading
			void load_rom(const char *file);
			void unload();

			// run loaded ROM
			void execute();

			// parse the next instruction
			void parse();

			// Instructions //
			// Exposed as member functions for testing purposes
			// TODO: cleanup interface
			void CLS();

			// Jump, Call, Return
			void JP(std::uint16_t addr);
			void CALL(std::uint16_t addr);
			void RET();

			// Skip if equal/not equal
			void SE(uint8_t a, uint8_t b);
			void SNE(uint8_t a, uint8_t b);

			// Load value
			void LD(std::uint8_t &a, std::uint8_t b);

			// Logical / Arithmetic
			void ADD(std::uint8_t &a, std::uint8_t b);
			static void OR(uint8_t &a, uint8_t b);
			static void AND(uint8_t &a, uint8_t b);
			static void XOR(uint8_t &a, uint8_t b);
			void SUB(uint8_t &a, uint8_t b);
			void SUBN(uint8_t &a, uint8_t b);
			void SHR(std::uint8_t &a);
			void SHL(uint8_t &a);

			// I Register
			void LDI(std::uint16_t a);
			void ADDI(uint8_t &a);

			// Random Number
			void RND(uint8_t &a, std::uint8_t b);

			// Draw
			void DRW(uint8_t x, uint8_t y, uint8_t n);

			// Keyboard instructions
			void SKP(uint8_t k);
			void SKNP(uint8_t k);
			// Wait for a keypress
			std::uint8_t KEY();

			// Sprites
			void LDSPR(uint8_t &a);

			// Binary Coded Decimal
			void BCD(uint8_t &a);

			// Store/read ranges
			void STORE(uint8_t &a);
			void READ(uint8_t &a);

	};

}

#endif //FANTASY8_CHIP8_CONTEXT_HPP
