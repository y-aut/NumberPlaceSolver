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

// 1�ł���ŉ��ʂ�bit�̉����琔����bit�ʒu�𓾂�B0��n���Ă͂Ȃ�Ȃ��B
inline int LSB32(u32 v) { ASSERTR(v, 0); unsigned long index; _BitScanForward(&index, v); return index; }
inline int LSB64(u64 v) { ASSERTR(v, 0); unsigned long index; _BitScanForward64(&index, v); return index; }

// 1�ł���ŏ�ʂ�bit�̉����琔����bit�ʒu�𓾂�B0��n���Ă͂Ȃ�Ȃ��B
inline int MSB32(u32 v) { ASSERTR(v, 0); unsigned long index; _BitScanReverse(&index, v); return index; }
inline int MSB64(u64 v) { ASSERTR(v, 0); unsigned long index; _BitScanReverse64(&index, v); return index; }

// �ŉ���bit��reset���閽�߁B
#define BLSR32(f) _blsr_u32(f)
#define BLSR64(f) _blsr_u64(f)

// 1�ł���ŉ���bit��1bit���o���āA�ォ�琔����bit�ʒu��Ԃ��B0��n���Ă͂Ȃ�Ȃ��B
inline int pop_lsb32(u32& b) { int index = 31 - LSB32(b); b = BLSR32(b); return index; }
inline int pop_lsb64(u64& b) { int index = 63 - LSB64(b); b = BLSR64(b); return index; }

// 1�ł���ŉ���bit��1bit���o���āA�ォ�琔����bit�ʒu��Ԃ��B0��n���Ă͂Ȃ�Ȃ��B���̒l�͕ς��Ȃ��B
inline int peek_lsb32(const u32 b) { return 31 - LSB32(b); }
inline int peek_lsb64(const u64 b) { return 63 - LSB64(b); }

// 1�ł���ŏ��bit��1bit���o���āA�ォ�琔����bit�ʒu��Ԃ��B0��n���Ă͂Ȃ�Ȃ��B���̒l�͕ς��Ȃ��B
inline int peek_msb32(const u32 b) { return 31 - MSB32(b); }
inline int peek_msb64(const u64 b) { return 63 - MSB64(b); }
