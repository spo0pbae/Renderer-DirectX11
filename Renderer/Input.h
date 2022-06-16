#pragma once

// prevent C++ from opening twice
#ifndef INPUT_H
#define INPUT_H

#include <bitset>

namespace end
{
	// Tells linker to pick one definition for use by all external references and to discard the rest
	// Otherwise linker error (LNK2005) (LNK1169)
	__declspec(selectany) std::bitset<256>	keys;

	// Mouse stuff
	__declspec(selectany) float currMouseX = 0;
	__declspec(selectany) float currMouseY = 0;
}

#endif