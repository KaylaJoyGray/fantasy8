#include "chip8_context.hpp"
#include <iostream>

int main(int argc, char** argv) {

	chip8::Context chip_8_CPU = chip8::Context();

	if (argc > 1) {
		chip_8_CPU.load_rom(argv[1]);
	} else {
		std::cout << argv[0] << " Too few arguments.\nUsage: fantasy8 file.chip8 (or file.bin etc.)" << std::endl;
		return 0;
	}

	chip_8_CPU.execute();

	return 0;
}
