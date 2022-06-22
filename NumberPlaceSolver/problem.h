#pragma once

#include <vector>

#include "board.h"

class problem : public board
{
	friend class trans;

	Number num[SQ_NB];
	bitboard emptyBB;

	// 各数字が入る可能性のあるマス
	bitboard candBB[NUM_NB];

	// num を再計算
	void calc_num();
	// cand を再計算
	void calc_cand();

public:
	problem() = default;
	problem(const Number _num[SQ_NB]);
	problem(const problem &src);

	Number at(const Square sq) const { ASSERTR(is_ok(sq), NUM_NB); return num[sq]; }
	bitboard empty_bb() const { return emptyBB; }
	bitboard cand_bb(const Number n) const { ASSERTR(is_ok(n), zero_bb()); return candBB[n]; }
	bitboard* cand_bb_ptr(const Number n) { ASSERTR(is_ok(n), nullptr); return &candBB[n]; }

	// いずれかの数字が入るマス
	bitboard any_cand_bb() const;
	vector<Number> get_cand(const Square sq) const;
	Number get_first_cand(const Square sq) const;

	// 候補数によるフィルター
	bitboard cand_one_bb() const;
	bitboard cand_two_bb() const;
	// 各マスの候補数を計算
	void get_cand_count_bb(bitboard bb[NUMBER_CNT + 1]) const;
	// 候補数が 1以上で最小のマスを返す
	bitboard cand_min_bb() const;

	void set(const Square sq, const Number n);
	void erase(const Square sq);
};
