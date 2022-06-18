#pragma once

#include "game.h"
#include "bitboard.h"

class board
{
	friend class trans;

protected:
	bitboard numBB[NUM_NB];

public:
	board() = default;
	board(const board& src) { memcpy(numBB, src.numBB, sizeof(numBB)); }

	bitboard num_bb(const Number n) const { ASSERTR(is_ok(n), zero_bb()); return numBB[n]; }

	void set(const Square sq, const Number n) { ASSERT(is_ok(sq) && is_ok(n)); numBB[n] |= sq; }
	void erase(const Square sq, const Number n) { ASSERT(is_ok(sq) && is_ok(n)); numBB[n] ^= sq; }

	string tostring() const;
};

bool is_ok(const board& b);
