//
// Created by kayla on 10/17/23.
// Defines the interface for the Chip-8 display
//

#ifndef FANTASY8_CHIP8_DISPLAY_HPP
#define FANTASY8_CHIP8_DISPLAY_HPP

#include "target_defines.hpp"
#include <bitset>

namespace chip8 {

	void display(const std::bitset<DISPLAY_WIDTH*DISPLAY_HEIGHT>& display_buffer);

}

#endif //FANTASY8_CHIP8_DISPLAY_HPP
