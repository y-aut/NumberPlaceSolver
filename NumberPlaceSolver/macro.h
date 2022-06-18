#pragma once

#define CS const static
#define CSI constexpr static int
#define CSUL constexpr static unsigned long long
#define CSF constexpr static float
#define CSD constexpr static double
#define CSB constexpr static bool
#define CSC constexpr static char

#ifdef NDEBUG
#define ASSERT(expression) (void)0
#define ASSERTR(expression, retval) (void)0
#else
#define ASSERT(expression) stop(expression)
#define ASSERTR(expression, retval) if (!stop(expression)) return retval
#endif

#ifndef NDEBUG
inline bool stop(const bool expression)
{
	if (!expression)
		return false;
	else
		return true;
};
#endif

#define UNREACHABLE ASSERT(false)

#ifdef NDEBUG
#define ERROR_EXIT abort()
#else
#define ERROR_EXIT ASSERT(false)
#endif

#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

template <typename T> inline T Max(const T v1, const T v2) { return v1 < v2 ? v2 : v1; }
template <typename T> inline T Min(const T v1, const T v2) { return v1 < v2 ? v1 : v2; }

inline int pow2(const int v1) { return v1 * v1; }

// FORMULA(1 <=, k, < n) ¨ 1 <= k && k < n
#define FORMULA(left_op,target,right_op) ((left_op (target)) && ((target) right_op))

#define ARRAY_LENGTH(ary) (sizeof(ary) / sizeof(ary[0]))

#define ENABLE_BASE_OPERATORS_ON(T)													\
	inline T operator+(const T d1, const int d2) { return T(int(d1) + d2); }		\
	inline T operator-(const T d1, const int d2) { return T(int(d1) - d2); }		\
	inline T operator+(const T d1, const T d2) { return T(int(d1) + int(d2)); }		\
	inline T operator-(const T d1, const T d2) { return T(int(d1) - int(d2)); }		\
	inline T operator-(const T d) { return T(-int(d)); }							\
	inline T& operator+=(T& d1, const T d2) { return d1 = d1 + d2; }				\
	inline T& operator-=(T& d1, const T d2) { return d1 = d1 - d2; }				\
	inline T& operator++(T& d) { return d = T(int(d) + 1); }						\
	inline T& operator--(T& d) { return d = T(int(d) - 1); }						\
	inline T operator++(T& d, int) { T prev = d; d = T(int(d) + 1); return prev; }	\
	inline T operator--(T& d, int) { T prev = d; d = T(int(d) - 1); return prev; }

// enum‚É‘Î‚µ‚Ärange for‚Å‰ñ‚¹‚é‚æ‚¤‚É‚·‚é‚½‚ß‚Ìhack
#define ENABLE_RANGE_OPERATORS_ON(T,ZERO,NB)	\
	ENABLE_BASE_OPERATORS_ON(T)					\
	inline T operator*(T d) { return d; }		\
	inline T begin(T) { return ZERO; }			\
	inline T end(T) { return NB; }


typedef unsigned short		u16;
typedef unsigned long		u32;
typedef unsigned long long	u64;

