#pragma once

// prevent C++ from opening twice
#ifndef INPUT_H
#define INPUT_H

#include <bitset>

// NOTE: __declspec(selectany) tells linker to pick one definition for use by all  
// external references and to discard the rest. Otherwise linker error (LNK2005) (LNK1169)
namespace end
{
	// Keystates
	__declspec(selectany) std::bitset<256>	keys;

	// Mouse positions
	__declspec(selectany) float currMouseX = 0.0f;
	__declspec(selectany) float currMouseY = 0.0f;
	__declspec(selectany) float prevMouseX = 0.0f;
	__declspec(selectany) float prevMouseY = 0.0f;
}

#endif