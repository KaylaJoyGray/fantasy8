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
				if (display_buffer[x*DISPLAY_WIDTH+y] == 1) {
					std::cout << 1;
				} else {
					std::cout << 0;
				}
			}
			std::cout << std::endl;
		}
		system("sleep 0.01");
	}
}