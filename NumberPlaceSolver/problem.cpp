#include "problem.h"

void problem::calc_num()
{
	for (auto n : Number()) {
		auto bb = num_bb(n);
		while (bb.isnot_empty())
			num[bb.pop()] = n;
	}
}

void problem::calc_cand()
{
	for (auto n : Number()) {
		candBB[n] = empty_bb();
		auto bb = num_bb(n);
		while (bb.isnot_empty()) {
			candBB[n] &= rev_effect_bb(bb.pop());
		}
	}
}

problem::problem(const Number _num[SQ_NB])
{
	for (Square sq : Square()) num[sq] = NUM_NONE;
	memset(numBB, 0, sizeof(numBB));
	emptyBB = all_bb();
	for (Square sq : Square()) {
		if ((num[sq] = _num[sq]) != NUM_NONE) {
			emptyBB ^= sq;
			numBB[at(sq)] |= sq;
		}
	}
	for (Number n : Number()) candBB[n] = empty_bb();
	auto tmp = ~empty_bb();
	while (tmp.isnot_empty()) {
		auto sq = tmp.pop();
		candBB[at(sq)] &= rev_effect_bb(sq);
	}
}

problem::problem(const problem& src) : board::board(src)
{
	memcpy(num, src.num, sizeof(num));
	memcpy(candBB, src.candBB, sizeof(candBB));
	emptyBB = src.emptyBB;
}

bitboard problem::any_cand_bb() const
{
	bitboard bb = cand_bb(NUM_ZERO);
	for (Number n = NUM_ZERO + 1; n < NUM_NB; ++n) bb |= cand_bb(n);
	return bb;
}

vector<Number> problem::get_cand(const Square sq) const
{
	auto ans = vector<Number>();
	for (Number n : Number())
		if (cand_bb(n).at(sq)) ans.push_back(n);
	return ans;
}

Number problem::get_first_cand(const Square sq) const
{
	for (Number n : Number())
		if (cand_bb(n).at(sq)) return n;
	return NUM_ZERO;
}

bitboard problem::cand_one_bb() const
{
	bitboard one = cand_bb(NUM_ZERO);
	bitboard more = zero_bb();

	for (Number n = NUM_ZERO + 1; n < NUM_NB; ++n) {
		more |= one & cand_bb(n);
		one = (one ^ cand_bb(n)) & ~more;
	}

	return one;
}

bitboard problem::cand_two_bb() const
{
	bitboard one = cand_bb(NUM_ZERO);
	bitboard two = zero_bb();
	bitboard more = zero_bb();

	for (Number n = NUM_ZERO + 1; n < NUM_NB; ++n) {
		more |= two & cand_bb(n);
		two = (two & ~more) | (one & cand_bb(n));
		one = (one | cand_bb(n)) & ~(two | more);
	}

	return two;
}

void problem::get_cand_count_bb(bitboard bb[NUMBER_CNT + 1]) const
{
	bb[0] = all_bb();
	for (int i = 1; i <= NUMBER_CNT; ++i)
		bb[i] = zero_bb();

	for (auto n : Number()) {
		for (int i = NUMBER_CNT; i > 0; --i) {
			bb[i] |= bb[i - 1] & cand_bb(n);
			bb[i - 1] &= ~cand_bb(n);
		}
	}
}

bitboard problem::cand_min_bb() const
{
	bitboard bb[NUMBER_CNT + 1];
	get_cand_count_bb(bb);

	for (int i = 1; i <= NUMBER_CNT; ++i) {
		if (bb[i].isnot_empty()) return bb[i];
	}
	ASSERT(false);
	return zero_bb();
}

void problem::set(const Square sq, const Number n)
{
	ASSERT(at(sq) == NUM_NONE);
	board::set(sq, n);
	num[sq] = n;
	emptyBB ^= sq;
	candBB[n] &= rev_effect_bb(sq);
	for (Number n1 : Number()) candBB[n1] &= ~sq_bb(sq);
}

void problem::erase(const Square sq)
{
	ASSERT(at(sq) != NUM_NONE);
	board::erase(sq, at(sq));
	num[sq] = NUM_NONE;
	emptyBB |= sq;
	calc_cand();
}
