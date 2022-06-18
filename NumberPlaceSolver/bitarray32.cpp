#include "bitarray32.h"

bitarray32 ZERO_32;
bitarray32 ALL_32;
bitarray32 EACH_32[BITARRAY32_BITCOUNT];
bitarray32 FILL_32[BITARRAY32_BITCOUNT];

void bitarray32::ready()
{
	ZERO_32 = bitarray32(0ul);
	ALL_32 = bitarray32(~0ul);
	EACH_32[0] = FILL_32[0] = bitarray32(1ul << (BITARRAY32_BITCOUNT - 1));
	for (int i = 1; i < BITARRAY32_BITCOUNT; ++i) {
		EACH_32[i] = EACH_32[i - 1] >> 1;
		FILL_32[i] = FILL_32[i - 1] | EACH_32[i];
	}
}

string bitarray32::tostring()
{
	string str = "";
	for (int i = 0; i < BITARRAY32_BITCOUNT; ++i)
		if (at(i)) str += to_string(i) + " ";
	if (str != "") str = str.substr(0, str.size() - 1);
	return str;
}
