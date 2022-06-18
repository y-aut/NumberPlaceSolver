#include "trans.h"

bitboard trans::apply(bitboard bb) const
{
	auto ans = bitboard();
	while (bb.isnot_empty()) {
		auto sq = bb.pop();
		ans |= sq_bb(apply(sq));
	}
	return ans;
}

void trans::apply(board& b) const
{
	for (auto n : Number())
		b.numBB[n] = apply(b.numBB[n]);
}

void trans::apply(problem& p) const
{
	apply(p);
	apply(p.emptyBB);
	for (auto n : Number())
		p.candBB[n] = apply(p.candBB[n]);
	p.calc_num();
}

Square rotate::apply(const Square sq) const
{
	if (type == ROTATE_CLOCK)
		return (FILE_NB - (File)rank_of(sq) - 1) | (Rank)file_of(sq);
	else
		return (File)rank_of(sq) | (RANK_NB - (Rank)file_of(sq) - 1);
}

File permutation::apply(const File f) const
{
	switch (type_of(g1))
	{
	case GRT_FILE:
		if (f == to_file(g1)) return to_file(g2);
		else if (f == to_file(g2)) return to_file(g1);
		else return f;

	case GRT_RANK:
		return f;

	default:
		auto f1 = file_of(topleft_of(to_block(g1)));
		auto f2 = file_of(topleft_of(to_block(g2)));
		if (FORMULA(f1 <= , f, < f1 + FILES_IN_BLOCK))
			return f + (f2 - f1);
		else if (FORMULA(f2 <= , f, < f2 + FILES_IN_BLOCK))
			return f + (f1 - f2);
		else return f;
	}
}

Rank permutation::apply(const Rank r) const
{
	switch (type_of(g1))
	{
	case GRT_FILE:
		return r;

	case GRT_RANK:
		if (r == to_rank(g1)) return to_rank(g2);
		else if (r == to_rank(g2)) return to_rank(g1);
		else return r;

	default:
		auto r1 = rank_of(topleft_of(to_block(g1)));
		auto r2 = rank_of(topleft_of(to_block(g2)));
		if (FORMULA(r1 <= , r, < r1 + RANKS_IN_BLOCK))
			return r + (r2 - r1);
		else if (FORMULA(r2 <= , r, < r2 + RANKS_IN_BLOCK))
			return r + (r1 - r2);
		else return r;
	}
}
