//
// Created by kayla on 10/17/23.
//

#include "chip8_display.hpp"
#include <iostream>
#include <cstdlib>

namespace chip8 {

	void display(const std::bitset<DISPLAY_WIDTH*DISPLAY_HEIGHT> &display_buffer)
	// this is the display function for the console version of the emulator
	{
		system("clear");
		for (auto y = 0; y < DISPLAY_HEIGHT; ++y) {

			for (auto x = 0; x < DISPLAY_WIDTH; ++x) {

				if (display_buffer[y*DISPLAY_WIDTH+x]) {
					std::cout << "\u2591\u2591";
				} else {
					std::cout << "  ";
				}
			}
			std::cout << std::endl;
		}
		system("sleep 0.01");
	}
}