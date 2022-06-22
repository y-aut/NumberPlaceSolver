#pragma once

#include <future>
#include <vector>

#include "problem.h"
#include "solution.h"
#include "predicate.h"

using namespace std;

namespace Search
{
	extern atomic<bool> flg_exit;
	extern bool flg_maxans;
	extern bool flg_output;

	void clear_sols();
	void clear_ans();
	
	void solve_all(const problem& p, const bool output, const bool random = false, const int max = -1);
	void solve(const problem& p, const bool output);

	void add_sol(problem& p, solution* sol);

	bool search_sol_alone_num(problem& p);
	bool search_sol_alone_num_in_group(problem& p);
	bool search_sol_occupy(problem& p);
	bool search_sol_reserve(problem& p);
	bool search_sol_xwing(problem& p);
	bool search_sol_xychain(problem& p);

	void make_problem_pre(const problem& p, const vector<const predicate*>& pre);
	void make_problem(const problem& p);
}
