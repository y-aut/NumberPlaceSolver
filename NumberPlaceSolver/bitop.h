#pragma once

#include "macro.h"

#include <nmmintrin.h>

#define POPCNT32(a) _mm_popcnt_u32(a)
#define POPCNT64(a) _mm_popcnt_u64(a)

#include <immintrin.h>

#define PEXT32(a,b) _pext_u32(a,b)
#define PEXT64(a,b) _pext_u64(a,b)

#define PDEP32(a,b) _pdep_u32(a,b)
#define PDEP64(a,b) _pdep_u64(a,b)

#include <intrin.h>

// 1である最下位のbitの下から数えたbit位置を得る。0を渡してはならない。
inline int LSB32(u32 v) { ASSERTR(v, 0); unsigned long index; _BitScanForward(&index, v); return index; }
inline int LSB64(u64 v) { ASSERTR(v, 0); unsigned long index; _BitScanForward64(&index, v); return index; }

// 1である最上位のbitの下から数えたbit位置を得る。0を渡してはならない。
inline int MSB32(u32 v) { ASSERTR(v, 0); unsigned long index; _BitScanReverse(&index, v); return index; }
inline int MSB64(u64 v) { ASSERTR(v, 0); unsigned long index; _BitScanReverse64(&index, v); return index; }

// 最下位bitをresetする命令。
#define BLSR32(f) _blsr_u32(f)
#define BLSR64(f) _blsr_u64(f)

// 1である最下位bitを1bit取り出して、上から数えたbit位置を返す。0を渡してはならない。
inline int pop_lsb32(u32& b) { int index = 31 - LSB32(b); b = BLSR32(b); return index; }
inline int pop_lsb64(u64& b) { int index = 63 - LSB64(b); b = BLSR64(b); return index; }

// 1である最下位bitを1bit取り出して、上から数えたbit位置を返す。0を渡してはならない。元の値は変わらない。
inline int peek_lsb32(const u32 b) { return 31 - LSB32(b); }
inline int peek_lsb64(const u64 b) { return 63 - LSB64(b); }

// 1である最上位bitを1bit取り出して、上から数えたbit位置を返す。0を渡してはならない。元の値は変わらない。
inline int peek_msb32(const u32 b) { return 31 - MSB32(b); }
inline int peek_msb64(const u64 b) { return 63 - MSB64(b); }
