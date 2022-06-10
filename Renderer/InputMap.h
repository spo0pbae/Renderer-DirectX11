#pragma once
#include <bitset>
#include <array>
namespace end
{
	enum e_input
	{
		FWD = 0,
		BACK,
		LEFT,
		RIGHT,
		COUNT
	};

	// Tells linker to pick one definition for use by all external references and to discard the rest
	// Otherwise compiler throws a linker error for duplicate definitions (LNK2005) (LNK1169)
	__declspec(selectany) std::bitset<end::e_input::COUNT>	keystate;
	__declspec(selectany) std::array<end::e_input, 256>		inputMap;
}