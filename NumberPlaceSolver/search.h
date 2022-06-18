#pragma once

#include <future>
#include <vector>

#include "problem.h"
#include "solution.h"

using namespace std;

namespace Search
{
	extern atomic<bool> flg_exit;
	extern bool flg_maxans;
	extern bool flg_output;

	void clear_sols();
	void clear_ans();
	
	void solve_all(const problem& p, const bool output, const bool random = false, const int max = -1);
	void solve_all_body(problem& p, const bool random, const int max);
	void solve(const problem& p, const bool output);

	void add_sol(problem& p, solution* sol);

	bool search_sol_alone_num(problem& p);
	bool search_sol_alone_num_in_group(problem& p);
	bool search_sol_occupy(problem& p);
	bool search_sol_occupy_body(problem& p, const int depth, const int depth_max, const Group g, Number* num, const bitboard bb);
	bool search_sol_reserve(problem& p);
	bool search_sol_xwing(problem& p);
	template <GroupType ForR>
	bool search_sol_xwing_body(problem& p, const int depth, const int depth_max, const Number num, Group* g1, const bitarray32 g2);

	void make_problem(const problem& p);
}
