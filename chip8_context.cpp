//
// Created by kayla on 10/13/23.
//

#include "chip8_context.hpp"
#include "chip8_display.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace chip8 {


	void Context::CLS()
	// Clear the console by setting the display buffer to 0
	{
		display_buffer.reset();
	}

	void Context::RET()
	// Return from a subroutine
	{
		PC = stack[--SP];     // pop value from the stack
	}

	void Context::JP(std::uint16_t addr)
	// Jump to address
	// Only the lowest 12 bits are used for the address
	{
		PC = addr & 0x0FFF;
	}

	void Context::CALL(std::uint16_t addr)
	// Call subroutine at address
	// Only the lowest 12 bits are used for the address
	{
		stack[SP] = PC;
		++SP;
		JP(addr);
	}

	void Context::SE(uint8_t a, uint8_t b)
	// Skip next instruction if a and b are equal
	{
		if (a == b) {
			PC += 2;
		}
	}

	void Context::SNE(std::uint8_t a, std::uint8_t b)
	// Skip next instruction if a and b are not equal
	{
		if (a != b) {
			PC += 2;
		}
	}

	void Context::LD(std::uint8_t &a, std::uint8_t b)
	// Set a to b
	{
		a = b;
	}

	void Context::ADD(std::uint8_t &a, std::uint8_t b)
	// Set a to a+b with overflow
	{
		if (static_cast<std::uint16_t>(a + b) > 255) {
			V[0x0F] = 1;
		} else {
			V[0x0F] = 0;
		}

		a += b;
	}

	void Context::OR(std::uint8_t &a, std::uint8_t b)
	// Perform a bitwise OR on a and b and store the result in a
	{
		a |= b;
	}

	void Context::AND(std::uint8_t &a, std::uint8_t b)
	// Perform a bitwise AND on a and b and store the result in a
	{
		a &= b;
	}

	void Context::XOR(std::uint8_t &a, std::uint8_t b)
	// Perform a bitwise XOR on a and b and store the result in a
	{
		a ^= b;
	}

	void Context::SUB(std::uint8_t &a, std::uint8_t b)
	// Subtract a from b and store the result in a
	// Set V[F] to borrow
	{
		if (a > b) {
			V[0x0F] = 1;
		} else {
			V[0x0F] = 0;
		}

		a -= b;
	}

	void Context::SHR(std::uint8_t &a)
	// Shift right and store the least significant bit in V[F]
	{
		V[0x0F] = a & 0b01;
		a >>= 1;
	}

	void Context::SUBN(std::uint8_t &a, std::uint8_t b)
	// Subtract a from b and store the result in a
	// Set V[F] to NOT borrow
	{
		if (b > a) {
			V[0x0F] = 1;
		} else {
			V[0x0F] = 0;
		}

		a = b - a;
	}

	void Context::SHL(std::uint8_t &a)
	// Shuffle left and store the most significant bit in V[F]
	{
		V[0x0F] = a >> 7;
		a <<= 1;
	}

	void Context::LDI(std::uint16_t a)
	// Load a into I
	{
		I = a & 0x0FFF;
	}

	void Context::RND(std::uint8_t &a, std::uint8_t b)
	// Set a = random byte & b
	{
		a = static_cast<std::uint8_t>(std::rand()) & b;
	}

	void Context::DRW(std::uint8_t x, std::uint8_t y, std::uint8_t n)
	// Display n-byte sprite at x, y
	// Set V[0xF] = collision
	{
		V[0xF] = 0;
		// iterate through each byte of the sprite (row)
		for (auto r = 0; r < n; ++r) {
			// iterate through each bit of each byte
			for (auto b = 0; b < 8; ++b) {

				std::uint16_t nx = x+b;
				std::uint16_t ny = y+r;

				if (nx > DISPLAY_WIDTH)
					nx = 0 + (nx - DISPLAY_WIDTH);

				if (ny > DISPLAY_HEIGHT)
					ny = 0 + (ny - DISPLAY_HEIGHT);

				auto sprtbit = ((memory[I + r] >> (8-b)) & 0b01);
				auto dispbit = display_buffer[ny * DISPLAY_WIDTH + nx];

				if (sprtbit && dispbit)
					V[0xF] = 1;

				display_buffer[ny * DISPLAY_WIDTH + nx] = dispbit ^ sprtbit;
			}
		}
	}

	void Context::SKP(uint8_t k)
	// Increments the Program Counter if key k is pressed.
	{
		if (keyboard[k] == 1)
			PC += 2;
	}

	void Context::SKNP(uint8_t k)
	// Increments the Program Counter if key k is not pressed.
	{
		if (keyboard[k] == 0)
			PC += 2;
	}

	std::uint8_t Context::KEY()
	// Waits for a keypress and stores the value in a
	{
		/*while (not input::Poll_Key(keyboard);) { // TODO: Handle keyboard events
			// wait
		}*/

		for (auto i = 0; i < 16; ++i) {
			if (keyboard[i] == 1)
				return i;
		}

		return 0;   // TODO: remove
	}

	void Context::ADDI(std::uint8_t &a)
	//  a and I are added, and the results are stored in I
	{
		I += a;
	}

	void Context::LDSPR(std::uint8_t &a)
	// Set I to the location for the hexadecimal sprite corresponding to a
	{
		I = a * 5;    // each sprite is 5 bytes long, and stored starting at memory location 0
	}

	void Context::BCD(std::uint8_t &a)
	// Store a binary coded decimal value of a at memory locations I, I+1, and I+2;
	{
		memory[I] = a / 100;
		memory[I+1] = (a / 10) % 10;
		memory[I+2] = a % 10;
	}

	void Context::STORE(std::uint8_t &a)
	// Store registers V[0] through V[a] in memory starting at location I
	{
		for (auto i = 0; i <= a; ++i) {
			memory[I+i] = V[i];
		}
	}

	void Context::READ(std::uint8_t &a)
	//  Read registers V[0] through V[x] from memory starting at location I
	{
		for (auto i = 0; i <= a; ++i) {
			V[i] = memory[I+i];
		}
	}

	void Context::load_rom(const char *file)
	// Load a ROM into "RAM" starting at the program counter
	{
		std::ifstream rom_file(file, std::ios::in | std::ios::binary);

		if (!rom_file.is_open()) {
			std::cout << "Error opening " << file << "! Quitting" << std::endl;
			return;
		}

		int ctr = 0;

		PC = 0x200; // reset program counter
		for (int i = PC; i < 4096 && !rom_file.eof(); ++i) {
			char byte;

			if (!rom_file.read(&byte, 1)) {
				break;
			}

			memory[i] = static_cast<std::uint8_t>(byte);
			++ctr;
		}

		std::cout << ctr << " bytes read into CHIP-8 RAM" << std::endl;

		rom_file.close();
		ldflag = true;
	}

	void Context::unload()
	// unload a ROM file
	{
		// go ahead and clear everything from the font files up for good measure
		// this includes some of the area "traditionally" reserved for the Chip-8 interpreter,
		// but there is nothing in there right now besides the font sprites
		// TODO: check this later for corrections
		for (auto i = 80; i < 4096; ++i) {
			memory[i] = 0;
		}

		ldflag = false;
	}

	void Context::parse()
	// Parse the next instruction
	{
		auto instruction = (static_cast<std::uint16_t>(memory[PC]) << 8) | memory[PC+1];

		std::uint8_t n1 = (instruction >> 12) & 0xF;
		std::uint8_t x = (instruction >> 8) & 0xF;
		std::uint8_t y = (instruction >> 4) & 0xF;
		std::uint8_t n4 = (instruction) & 0xF;

		std::uint8_t byte = instruction & 0xF;

		if (instruction == 0x00E0) {
			CLS();
		} else
		if (instruction == 0x00EE) {
			RET();
		} else
		if (n1 == 0x1){
			JP(instruction);
		} else
		if (n1 == 0x2) {
			CALL(instruction);
		} else
		if (n1 == 0x3) {
			SE(V[x], byte);
		} else
		if (n1 == 0x4) {
			SNE(V[x], byte);
		} else
		if (n1 == 0x5) {
			SE(V[x], V[y]);
		} else
		if (n1 == 0x6) {
			LD(V[x], byte);
		} else
		if (n1 == 0x7) {
			ADD(V[x], byte);
		} else
		if (n1 == 0x8 && n4 == 0) {
			LD(V[x], V[y]);
		} else
		if (n1 == 0x8 && n4 == 0x1) {
			OR(V[x], V[y]);
		} else
		if (n1 == 0x8 && n4 == 0x2) {
			AND(V[x], V[y]);
		} else
		if (n1 == 0x8 && n4 == 0x3) {
			XOR(V[x], V[y]);
		} else
		if (n1 == 0x8 && n4 == 0x4) {
			ADD(V[x], V[y]);
		} else
		if (n1 == 0x8 && n4 == 0x5) {
			SUB(V[x], V[y]);
		} else
		if (n1 == 0x8 && n4 == 0x6) {
			SHR(V[x]);
		} else
		if (n1 == 0x8 && n4 == 0x7) {
			SUBN(V[x], V[y]);
		} else
		if (n1 == 0x8 && n4 == 0xE) {
			SHL(V[x]);
		} else
		if (n1 == 0x9) {
			SNE(V[x], V[y]);
		} else
		if (n1 == 0xA) {
			LDI(instruction);
		} else
		if (n1 == 0xB) {
			JP(V[0]+(instruction & 0x0FFF));
		} else
		if (n1 == 0xC) {
			RND(V[x], 0);
		} else
		if (n1 == 0xD) {
			DRW(V[x], V[y], n4);
		} else
		if (n1 == 0xE && byte == 0x9E) {
			SKP(V[x]);
		} else
		if (n1 == 0xE && byte == 0xA1 ) {
			SKNP(V[x]);
		} else
		if (n1 == 0xF && byte == 0x07) {
			LD(V[x], DT);
		} else
		if (n1 == 0xF && byte == 0x0A) {
			LD(V[x], KEY());
		} else
		if (n1 == 0xF && byte == 0x15) {
			LD(DT, V[x]);
		} else
		if (n1 == 0xF && byte == 0x18) {
			LD(ST, V[x]);
		} else
		if (n1 == 0xF && byte == 0x1E) {
			ADDI(V[x]);
		} else
		if (n1 == 0xF && byte == 0x29) {
			LDSPR(V[x]);
		} else
		if (n1 == 0xF && byte == 0x33) {
			BCD(V[x]);
		} else
		if (n1 == 0xF && byte == 0x55) {
			STORE(V[x]);
		} else
		if (n1 == 0xF && byte == 0x65) {
			READ(V[x]);
		}
	}

	void Context::execute()
	// run a loaded ROM
	{
		if (!ldflag) {
			std::cout << "No ROM loaded" << std::endl;
			return;
		}

		while (true) {
			parse();
			chip8::display(display_buffer);
			PC += 2;
		}
	}

	const std::bitset<DISPLAY_WIDTH*DISPLAY_HEIGHT> &Context::get_display_buffer() const
	// getter for the display buffer
	{
		return display_buffer;
	}
}