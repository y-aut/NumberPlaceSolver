#pragma once

#include <vector>

#include "board.h"

class problem : public board
{
	friend class trans;

	Number num[SQ_NB];
	bitboard emptyBB;

	// �e����������\���̂���}�X
	bitboard candBB[NUM_NB];

	// num ���Čv�Z
	void calc_num();
	// cand ���Čv�Z
	void calc_cand();

public:
	problem() = default;
	problem(const Number _num[SQ_NB]);
	problem(const problem &src);

	Number at(const Square sq) const { ASSERTR(is_ok(sq), NUM_NB); return num[sq]; }
	bitboard empty_bb() const { return emptyBB; }
	bitboard cand_bb(const Number n) const { ASSERTR(is_ok(n), zero_bb()); return candBB[n]; }
	bitboard* cand_bb_ptr(const Number n) { ASSERTR(is_ok(n), nullptr); return &candBB[n]; }

	// �����ꂩ�̐���������}�X
	bitboard any_cand_bb() const;
	vector<Number> get_cand(const Square sq) const;
	Number get_first_cand(const Square sq) const;

	// ��␔�ɂ��t�B���^�[
	bitboard cand_one_bb() const;
	bitboard cand_two_bb() const;
	// �e�}�X�̌�␔���v�Z
	void get_cand_count_bb(bitboard bb[NUMBER_CNT + 1]) const;
	// ��␔�� 1�ȏ�ōŏ��̃}�X��Ԃ�
	bitboard cand_min_bb() const;

	void set(const Square sq, const Number n);
	void erase(const Square sq);
};
