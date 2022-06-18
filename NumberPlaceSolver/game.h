#pragma once

#include <string>

#include "macro.h"

using namespace std;

CSI CELL_XCNT = 9;
CSI CELL_YCNT = 9;
CSI CELL_CNT = CELL_XCNT * CELL_YCNT;

CSI BLOCK_XCNT = 3;
CSI BLOCK_YCNT = 3;
CSI BLOCK_CNT = BLOCK_XCNT * BLOCK_YCNT;

CSI FILES_IN_BLOCK = 3;
CSI RANKS_IN_BLOCK = 3;
CSI CELLS_IN_BLOCK = FILES_IN_BLOCK * RANKS_IN_BLOCK;

CSI CELLS_IN_GROUP = 9;

CSI NUMBER_CNT = 9;

#pragma region 数値

enum Number
{
	NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
	NUM_NB, NUM_ZERO = NUM_1, NUM_NONE = -1,
};
ENABLE_RANGE_OPERATORS_ON(Number, NUM_ZERO, NUM_NB)

inline bool is_ok(const Number n) { return FORMULA(NUM_ZERO <= , n, < NUM_NB); }

inline char to_char(const Number n) { return (char)('1' + n); }
inline string to_string(const Number n) { return { to_char(n) }; }

#pragma endregion

#pragma region 縦列

enum File
{
	FILE_0, FILE_1, FILE_2, FILE_3, FILE_4, FILE_5, FILE_6, FILE_7, FILE_8,
	FILE_NB, FILE_ZERO = 0,
};
ENABLE_RANGE_OPERATORS_ON(File, FILE_ZERO, FILE_NB)

inline bool is_ok(const File f) { return FORMULA(FILE_ZERO <= , f, < FILE_NB); }

inline string to_string(const File f) { return { (char)('A' + f) }; }

#pragma endregion

#pragma region 横列

enum Rank
{
	RANK_0, RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8,
	RANK_NB, RANK_ZERO = 0,
};
ENABLE_RANGE_OPERATORS_ON(Rank, RANK_ZERO, RANK_NB)

inline bool is_ok(const Rank r) { return FORMULA(RANK_ZERO <= , r, < RANK_NB); }

inline string to_string(const Rank r) { return { (char)('1' + r) }; }

#pragma endregion

#pragma region ブロック

enum Block
{
	// 左上から右方向に0, 1, ...
	BLOCK_00, BLOCK_01, BLOCK_02,
	BLOCK_10, BLOCK_11, BLOCK_12,
	BLOCK_20, BLOCK_21, BLOCK_22,
	BLOCK_NB, BLOCK_ZERO = 0,
};
ENABLE_RANGE_OPERATORS_ON(Block, BLOCK_ZERO, BLOCK_NB)

inline bool is_ok(const Block b) { return FORMULA(BLOCK_ZERO <= , b, < BLOCK_NB); }

inline string to_string(const Block b) { return { (char)('a' + b) }; }

#pragma endregion

#pragma region マス

enum Square
{
	// 各マスを表す定数
	SQ_00, SQ_01, SQ_02, SQ_03, SQ_04, SQ_05, SQ_06, SQ_07, SQ_08,
	SQ_10, SQ_11, SQ_12, SQ_13, SQ_14, SQ_15, SQ_16, SQ_17, SQ_18,
	SQ_20, SQ_21, SQ_22, SQ_23, SQ_24, SQ_25, SQ_26, SQ_27, SQ_28,
	SQ_30, SQ_31, SQ_32, SQ_33, SQ_34, SQ_35, SQ_36, SQ_37, SQ_38,
	SQ_40, SQ_41, SQ_42, SQ_43, SQ_44, SQ_45, SQ_46, SQ_47, SQ_48,
	SQ_50, SQ_51, SQ_52, SQ_53, SQ_54, SQ_55, SQ_56, SQ_57, SQ_58,
	SQ_60, SQ_61, SQ_62, SQ_63, SQ_64, SQ_65, SQ_66, SQ_67, SQ_68,
	SQ_70, SQ_71, SQ_72, SQ_73, SQ_74, SQ_75, SQ_76, SQ_77, SQ_78,
	SQ_80, SQ_81, SQ_82, SQ_83, SQ_84, SQ_85, SQ_86, SQ_87, SQ_88,

	// ゼロと末尾
	SQ_NB, SQ_ZERO = 0,
};
ENABLE_RANGE_OPERATORS_ON(Square, SQ_ZERO, SQ_NB)

inline Square operator|(const File f, const Rank r) { return Square(f + r * CELL_XCNT); }

inline File file_of(const Square sq) { return File(sq % CELL_XCNT); }
inline Rank rank_of(const Square sq) { return Rank(sq / CELL_XCNT); }
inline Block block_of(const Square sq) { return Block(file_of(sq) / FILES_IN_BLOCK + rank_of(sq) / RANKS_IN_BLOCK * BLOCK_XCNT); }

inline Square topleft_of(const Block b) {
	return (File)((b % BLOCK_XCNT) * FILES_IN_BLOCK) | (Rank)((b / BLOCK_XCNT) * RANKS_IN_BLOCK);
}

inline bool is_ok(const Square sq) { return FORMULA(SQ_ZERO <= , sq, < SQ_NB); }

inline string to_string(const Square sq) { return { (char)('A' + file_of(sq)), (char)('1' + rank_of(sq)) }; }

#pragma endregion

#pragma region グループ

enum GroupType
{
	GRT_FILE, GRT_RANK, GRT_BLOCK,
	GRT_NB, GRT_ZERO = 0,
};
ENABLE_RANGE_OPERATORS_ON(GroupType, GRT_ZERO, GRT_NB)

typedef GroupType GRT;

enum Group
{
	GR_FILE_0, GR_FILE_1, GR_FILE_2, GR_FILE_3, GR_FILE_4, GR_FILE_5, GR_FILE_6, GR_FILE_7, GR_FILE_8,

	GR_RANK_0, GR_RANK_1, GR_RANK_2, GR_RANK_3, GR_RANK_4, GR_RANK_5, GR_RANK_6, GR_RANK_7, GR_RANK_8,

	GR_BLOCK_00, GR_BLOCK_01, GR_BLOCK_02,
	GR_BLOCK_10, GR_BLOCK_11, GR_BLOCK_12,
	GR_BLOCK_20, GR_BLOCK_21, GR_BLOCK_22,

	GR_NB, GR_ZERO = 0,
	GR_FILE_ZERO = GR_FILE_0, GR_RANK_ZERO = GR_RANK_0, GR_BLOCK_ZERO = GR_BLOCK_00,
	GR_FILE_NB = GR_RANK_ZERO, GR_RANK_NB = GR_BLOCK_ZERO, GR_BLOCK_NB = GR_NB,
};
ENABLE_RANGE_OPERATORS_ON(Group, GR_ZERO, GR_NB)

inline bool is_ok(const Group g) { return FORMULA(GR_ZERO <= , g, < GR_NB); }

inline GRT type_of(const Group g) { ASSERTR(is_ok(g), GRT_ZERO); return (GRT)(g / CELLS_IN_GROUP); }
inline Group to_group(const File f) { return GR_FILE_ZERO + f; }
inline Group to_group(const Rank r) { return GR_RANK_ZERO + r; }
inline Group to_group(const Block b) { return GR_BLOCK_ZERO + b; }
inline File to_file(const Group g) { ASSERTR(type_of(g) == GRT_FILE, FILE_ZERO); return (File)(g - GR_FILE_ZERO); }
inline Rank to_rank(const Group g) { ASSERTR(type_of(g) == GRT_RANK, RANK_ZERO); return (Rank)(g - GR_RANK_ZERO); }
inline Block to_block(const Group g) { ASSERTR(type_of(g) == GRT_BLOCK, BLOCK_ZERO); return (Block)(g - GR_BLOCK_ZERO); }

template <GroupType grt>
inline Group gr_zero() {
	return grt == GRT_FILE ? GR_FILE_ZERO
		: (grt == GRT_RANK ? GR_RANK_ZERO : GR_BLOCK_ZERO);
}
template <GroupType grt>
inline Group gr_nb() {
	return grt == GRT_FILE ? GR_FILE_NB
		: (grt == GRT_RANK ? GR_RANK_NB : GR_BLOCK_NB);
}
template <GroupType grt>
inline Group group_of(const Square sq) {
	return grt == GRT_FILE ? to_group(file_of(sq))
		: (grt == GRT_RANK ? to_group(rank_of(sq)) : to_group(block_of(sq)));
}

string to_string(const Group g);

#pragma endregion
