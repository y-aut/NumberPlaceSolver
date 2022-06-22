#pragma once

#include <string>
#include <random>

#include "game.h"
#include "bitop.h"
#include "macro.h"

using namespace std;

struct bitboard;

// 空のビットボード
extern bitboard ZERO_BB;
// 全マスが立っているビットボード
extern bitboard ALL_BB;
// 各マスが立っているビットボード
extern bitboard SQ_BB[SQ_NB];
// SQ_ZERO から i までのビットが立っているビットボード
extern bitboard FILL_BB[SQ_NB];
// 各グループのビットボード
extern bitboard GROUP_BB[GR_NB];
// あるマスに対してその列，行，ブロックが立っているビットボード
extern bitboard EFFECT_BB[SQ_NB];
extern bitboard REV_EFFECT_BB[SQ_NB];

CSI BB_P1_NB = 54;	// p[0]の使用ビット数
/*
	ビットボードクラス
	ビット配列：
		| 0| 1| 2| 3| ... | 8| : r = 0
		| 9|10|11|12| ... |17| : r = 1
		（略）
		|45|46|47|48| ... |53| : r = 5

		| 0| 1| 2| 3| ... | 8| : r = 6
		| 9|10|11|12| ... |17| : r = 7
		|18|19|20|21| ... |26| : r = 8
		f[0][1][2][3] ... [8]
*/
struct alignas(16) bitboard
{
private:
	bitboard(const u64 p0, const u64 p1) { p[0] = p0; p[1] = p1; }

public:
	bitboard() = default;
	bitboard(const Square sq) { *this = SQ_BB[sq]; }

	// 各種定数ビットボードを初期化
	static void ready();

	bool is_ok() const { return (*this & bitboard(~ALL_BB.p[0], ~ALL_BB.p[1])).is_empty(); }

	bool isnot_empty() const { return p[0] || p[1]; }
	bool is_empty() const { return !isnot_empty(); }

	bool at(const Square sq) const { ASSERT(is_ok()); return (*this & sq).isnot_empty(); }

	// ビットを数える
	int popcnt() const { ASSERT(is_ok()); return static_cast<int>(POPCNT64(p[0]) + POPCNT64(p[1])); }

	// 最下位ビットの升を返し、そのビットを削除する
	Square pop();

	// ランダムに立っているマスを返し、そのビットを削除する
	Square pop_random(mt19937 mt);

	// 最下位ビットの升を返す
	Square peek_lsb() const;

	// 最上位ビットの升を返す
	Square peek_msb() const;

	// ランダムに立っているマスを返す
	Square peek_random(mt19937 mt) const;

	// bitboardの1の升を'*'、0の升を'.'として表示する。デバッグ用。
	string tostring() const;

	// 演算子オーバーロード
	bool operator==(const bitboard b) const { ASSERT(b.is_ok()); return p[0] == b.p[0] && p[1] == b.p[1]; }
	bool operator!=(const bitboard b) const { ASSERT(b.is_ok()); return !(*this == b); }
	bitboard operator<<(const int x) const { return bitboard(p[0] << x, p[1] << x); }
	bitboard operator>>(const int x) const { return bitboard(p[0] >> x, p[1] >> x) & ALL_BB; }
	bitboard operator&(const bitboard b) const { return bitboard(p[0] & b.p[0], p[1] & b.p[1]); }
	bitboard operator|(const bitboard b) const { return bitboard(p[0] | b.p[0], p[1] | b.p[1]); }
	bitboard operator^(const bitboard b) const { ASSERT(b.is_ok()); return bitboard(p[0] ^ b.p[0], p[1] ^ b.p[1]); }
	bitboard& operator&=(const bitboard b) { ASSERT(b.is_ok()); return *this = *this & b; }
	bitboard& operator|=(const bitboard b) { ASSERT(b.is_ok()); return *this = *this | b; }
	bitboard& operator^=(const bitboard b) { ASSERT(b.is_ok()); return *this = *this ^ b; }

	bitboard operator~() const { return *this ^ ALL_BB; }

	bitboard operator&(const Square sq) const { return *this & SQ_BB[sq]; }
	bitboard operator|(const Square sq) const { return *this | SQ_BB[sq]; }
	bitboard operator^(const Square sq) const { return *this ^ SQ_BB[sq]; }
	bitboard& operator&=(const Square sq) { return *this = *this & sq; }
	bitboard& operator|=(const Square sq) { return *this = *this | sq; }
	bitboard& operator^=(const Square sq) { return *this = *this ^ sq; }

private:
	u64 p[2];
};

// ビットボード定数を参照する関数
inline bitboard zero_bb() { return ZERO_BB; }
inline bitboard all_bb() { return ALL_BB; }
inline bitboard sq_bb(const Square sq) { ASSERTR(is_ok(sq), zero_bb()); return SQ_BB[sq]; }
inline bitboard fill_bb(const Square sq) { ASSERTR(is_ok(sq), zero_bb()); return FILL_BB[sq]; }
inline bitboard group_bb(const Group g) { ASSERTR(is_ok(g), zero_bb()); return GROUP_BB[g]; }
inline bitboard file_bb(const File f) { ASSERTR(is_ok(f), zero_bb()); return group_bb(to_group(f)); }
inline bitboard rank_bb(const Rank r) { ASSERTR(is_ok(r), zero_bb()); return group_bb(to_group(r)); }
inline bitboard block_bb(const Block b) { ASSERTR(is_ok(b), zero_bb()); return group_bb(to_group(b)); }
inline bitboard effect_bb(const Square sq) { ASSERTR(is_ok(sq), zero_bb()); return EFFECT_BB[sq]; }
inline bitboard rev_effect_bb(const Square sq) { ASSERTR(is_ok(sq), zero_bb()); return REV_EFFECT_BB[sq]; }
inline bitboard outer_effect_bb(const Square sq) { return effect_bb(sq) ^ sq; }
inline bitboard rev_outer_effect_bb(const Square sq) { return rev_effect_bb(sq) ^ sq; }
