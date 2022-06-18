#pragma once

#include <string>

#include "game.h"
#include "bitop.h"
#include "macro.h"

CSI BITARRAY32_BITCOUNT = 32;

struct bitarray32;

extern bitarray32 ZERO_32;
extern bitarray32 ALL_32;
extern bitarray32 EACH_32[BITARRAY32_BITCOUNT];
// 上から連続して(i+1)ビットが1
extern bitarray32 FILL_32[BITARRAY32_BITCOUNT];

struct bitarray32
{
public:
	bitarray32() = default;
	bitarray32(const u32 p_) : p(p_) {}

	// 各種定数ビットボードを初期化
	static void ready();

	bool isnot_empty() const { return p; }
	bool is_empty() const { return !isnot_empty(); }

	bool at(const int i) const { return (*this & EACH_32[i]).isnot_empty(); }

	// ビットを数える
	int popcnt() const { return static_cast<int>(POPCNT32(p)); }

	// 最下位ビットの上から数えたインデックスを返し、そのビットを削除する
	int pop() { return pop_lsb32(p); }

	// 最下位ビットの上から数えたインデックスを返す
	int peek() const { return peek_lsb32(p); }

	// intのリストとして文字列化する
	string tostring();

	// 演算子オーバーロード
	bool operator==(const bitarray32 b) const { return p == b.p; }
	bool operator!=(const bitarray32 b) const { return !(*this == b); }
	bitarray32 operator<<(const int x) const { return bitarray32(p << x); }
	bitarray32 operator>>(const int x) const { return bitarray32(p >> x); }
	bitarray32 operator&(const bitarray32 b) const { return bitarray32(p & b.p); }
	bitarray32 operator|(const bitarray32 b) const { return bitarray32(p | b.p); }
	bitarray32 operator^(const bitarray32 b) const { return bitarray32(p ^ b.p); }
	bitarray32& operator&=(const bitarray32 b) { return *this = *this & b; }
	bitarray32& operator|=(const bitarray32 b) { return *this = *this | b; }
	bitarray32& operator^=(const bitarray32 b) { return *this = *this ^ b; }

	bitarray32 negate(const int count) const { return bitarray32(~p & FILL_32[count - 1].p); }

private:
	u32 p;
};

inline bitarray32 zero_32() { return ZERO_32; }
inline bitarray32 all_32() { return ALL_32; }
inline bitarray32 each_32(const int i) { ASSERTR(FORMULA(0 <= , i, < BITARRAY32_BITCOUNT), zero_32()); return EACH_32[i]; }
inline bitarray32 fill_32(const int i) { ASSERTR(FORMULA(0 <= , i, < BITARRAY32_BITCOUNT), zero_32()); return FILL_32[i]; }
