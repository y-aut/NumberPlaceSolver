#include "game.h"
#include "bitboard.h"

bitboard ZERO_BB;
bitboard ALL_BB;
bitboard SQ_BB[SQ_NB];
bitboard FILL_BB[SQ_NB];
bitboard GROUP_BB[GR_NB];
bitboard EFFECT_BB[SQ_NB];
bitboard REV_EFFECT_BB[SQ_NB];

void bitboard::ready()
{
	ZERO_BB = bitboard(0, 0); ALL_BB = ZERO_BB;
	memset(GROUP_BB, 0, sizeof(GROUP_BB));

	SQ_BB[0] = bitboard(0x8000000000000000, 0);
	SQ_BB[BB_P1_NB] = bitboard(0, 0x8000000000000000);
	for (int i = 1; i < BB_P1_NB; ++i) SQ_BB[i] = bitboard(SQ_BB[i - 1].p[0] >> 1, 0);
	for (int i = BB_P1_NB + 1; i < SQ_NB; ++i) SQ_BB[i] = bitboard(0, SQ_BB[i - 1].p[1] >> 1);
	FILL_BB[0] = sq_bb(SQ_ZERO);
	for (Square sq = SQ_ZERO + 1; sq < SQ_NB; ++sq) FILL_BB[sq] = FILL_BB[sq - 1] | sq_bb(sq);
	for (Square sq : Square()) {
		ALL_BB |= sq;
		GROUP_BB[to_group(file_of(sq))] |= sq;
		GROUP_BB[to_group(rank_of(sq))] |= sq;
		GROUP_BB[to_group(block_of(sq))] |= sq;
	}
	for (Square sq : Square()) {
		EFFECT_BB[sq] = file_bb(file_of(sq)) | rank_bb(rank_of(sq)) | block_bb(block_of(sq));
		REV_EFFECT_BB[sq] = ~effect_bb(sq);
	}
}

Square bitboard::pop()
{
	ASSERT(is_ok());
	Square sq;
	u64 p1 = p[1];
	if (p1 != 0) { sq = Square(pop_lsb64(p1) + BB_P1_NB); p[1] = p1; }
	else { u64 p0 = p[0];  sq = Square(pop_lsb64(p0)); p[0] = p0; }
	return sq;
}

Square bitboard::pop_random(mt19937 mt)
{
	ASSERT(is_ok());
	auto sq = peek_random(mt);
	*this ^= sq_bb(sq);
	return sq;
}

Square bitboard::peek_lsb() const
{
	ASSERT(is_ok());
	if (p[1] != 0) return Square(peek_lsb64(p[1]) + BB_P1_NB);
	else return Square(peek_lsb64(p[0]));
}

Square bitboard::peek_msb() const
{
	ASSERT(is_ok());
	if (p[0] != 0) return Square(peek_msb64(p[0]));
	else return Square(peek_msb64(p[1]) + BB_P1_NB);
}

Square bitboard::peek_random(mt19937 mt) const
{
	ASSERT(is_ok());
	// [0, SQ_NB) ‚Ì—”‚ð¶¬
	auto sq = (Square)(mt() % SQ_NB);

	auto upper = *this & fill_bb(sq);
	if (upper.isnot_empty()) return upper.peek_lsb();
	else return peek_msb();
}

string bitboard::tostring() const
{
	ASSERT(is_ok());
	string ans = "";
	for (Rank r : Rank()) {
		for (File f : File())
			ans += at(f | r) ? "*" : ".";
		ans += "\n";
	}
	return ans;
}
