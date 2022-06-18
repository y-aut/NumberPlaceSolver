#include <iostream>
#include <random>
#include <chrono>

#include "search.h"

atomic<bool> Search::flg_exit;
bool Search::flg_maxans;
bool Search::flg_output;
vector<solution*> sols;
vector<problem*> ans;
std::random_device rnd;
std::mt19937 mt(rnd());

void Search::clear_sols()
{
	for (auto ptr : sols) delete ptr;
	sols.clear();
}

void Search::clear_ans()
{
	for (auto ptr : ans) delete ptr;
	ans.clear();
}

void Search::solve_all(const problem& p, const bool output, const bool random, const int max)
{
	flg_exit = false;
	flg_maxans = false;
	flg_output = output;
	clear_ans();
	auto cpy = p;
	solve_all_body(cpy, random, max);
	if (flg_output) {
		if (flg_exit)
			cout << "stopped" << endl;
		else if (flg_maxans)
			cout << "max" << endl;
		else
			cout << "thatsall" << endl;
	}
}

void Search::solve_all_body(problem& p, const bool random, const int max)
{
	if (flg_exit || flg_maxans) return;

	if (p.empty_bb().is_empty()) {
		ASSERT(is_ok(p));
		// ローカル変数のポインタを保持しない！
		ans.push_back(new problem(p));
		if (flg_output)
			cout << "ans " << p.tostring() << endl;
		if (ans.size() == max)
			flg_maxans = true;
	}
	else if ((p.empty_bb() & ~p.any_cand_bb()).isnot_empty()) {
		// 解無し
		return;
	}
	else {
		// 候補数が少ないマスを優先して取り出す
		auto sq = p.cand_min_bb().peek_msb();
		auto cand = p.get_cand(sq);
		if (random) {
			std::shuffle(cand.begin(), cand.end(), mt);
		}
		for (auto n : p.get_cand(sq)) {
			auto cpy = p;
			cpy.set(sq, n);
			solve_all_body(cpy, random, max);
			if (flg_exit || flg_maxans) return;
		}
	}
}

void Search::solve(const problem& pro, const bool output)
{
	flg_exit = false;
	flg_output = output;
	clear_sols();
	auto p = pro;

	while (p.empty_bb().isnot_empty())
	{
		if (flg_exit) {
			cout << "stopped" << endl;
			return;
		}

		if (search_sol_alone_num(p)) continue;
		if (search_sol_alone_num_in_group(p)) continue;
		if (search_sol_occupy(p)) continue;
		if (search_sol_reserve(p)) continue;
		if (search_sol_xwing(p)) continue;

		// no solution
		if (flg_output) {
			cout << "giveup" << endl;
			cout << "info board " << p.tostring() << endl;
		}
		return;
	}

	if (flg_output) {
		cout << "solved" << endl;
		cout << "info board " << p.tostring() << endl;
	}
}

void Search::add_sol(problem& p, solution* sol)
{
	sol->apply(p);
	if (flg_output)
		cout << "sol " << sol->tostring() << endl;
	sols.push_back(sol);
}

bool Search::search_sol_alone_num(problem& p)
{
	auto one = p.cand_one_bb();
	if (one.isnot_empty()) {
		auto sq = one.peek_msb();
		auto num = p.get_first_cand(sq);
		add_sol(p, new sol_alone_num(sq, num));
		return true;
	}
	return false;
}

bool Search::search_sol_alone_num_in_group(problem& p)
{
	for (auto g : Group()) {
		for (auto n : Number()) {
			auto tmp = p.cand_bb(n) & group_bb(g);
			if (tmp.popcnt() == 1) {
				add_sol(p, new sol_alone_num_in_group(tmp.pop(), n, g));
				return true;
			}
		}
	}
	return false;
}

bool Search::search_sol_occupy(problem& p)
{
	Number num[NUMBER_CNT];

	for (auto g : Group()) {
		int depth_max = (p.empty_bb() & group_bb(g)).popcnt() - 2;
		if (search_sol_occupy_body(p, 0, depth_max, g, num, zero_bb()))
			return true;
	}
	
	return false;
}

bool Search::search_sol_occupy_body(problem& p, const int depth, const int depth_max, const Group g, Number* num, const bitboard bb)
{
	auto cnt = bb.popcnt();
	if (cnt == depth && cnt > 0) {
		// 消す候補があるか
		auto flgValid = false;
		auto tmp = bb;
		do {
			if (p.get_cand(tmp.pop()).size() > cnt) {
				flgValid = true;
				break;
			}
		} while (tmp.isnot_empty());

		if (flgValid) {
			bitarray32 nums = zero_32();
			for (int i = 0; i < depth; ++i)
				nums |= each_32(num[i]);
			for (auto n : Number())
				if ((p.num_bb(n) & group_bb(g)).isnot_empty()) nums |= each_32(n);

			add_sol(p, new sol_occupy(p.empty_bb() & (group_bb(g) ^ bb), nums.negate(NUMBER_CNT), g));
			return true;
		}
	}
	if (cnt > depth_max) return false;
	if (depth == depth_max) return false;

	for (num[depth] = depth == 0 ? NUM_ZERO : (num[depth - 1] + 1); num[depth] < NUM_NB; ++num[depth]) {
		if ((p.num_bb(num[depth]) & group_bb(g)).is_empty()) {
			auto tmp = bb | (p.cand_bb(num[depth]) & group_bb(g));
			if (search_sol_occupy_body(p, depth + 1, depth_max, g, num, tmp))
				return true;
		}
	}

	return false;
}

bool Search::search_sol_reserve(problem& p)
{
	for (auto g1 : Group()) {
		for (auto g2 : Group()) {
			if ((type_of(g1) == GRT_BLOCK) != (type_of(g2) == GRT_BLOCK)
				&& (group_bb(g1) & group_bb(g2)) != zero_bb())
			{
				for (auto n : Number()) {
					auto tmp = p.cand_bb(n) & group_bb(g1);
					if (tmp != zero_bb()	// 埋まっていない数字で
						&& (tmp & ~group_bb(g2)).is_empty()		// g1で入るのがg2上のマスだけで
						&& (p.cand_bb(n) & ~group_bb(g1) & group_bb(g2)).isnot_empty()) {	// g2で入るのはg1上のマスだけではない
						add_sol(p, new sol_reserve(tmp, n, g1, g2));
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool Search::search_sol_xwing(problem& p)
{
	Group g1[CELLS_IN_GROUP];

	for (auto n : Number()) {
		int depth_max = CELLS_IN_GROUP - p.num_bb(n).popcnt();
		if (search_sol_xwing_body<GRT_FILE>(p, 0, depth_max, n, g1, zero_32()))
			return true;
		if (search_sol_xwing_body<GRT_RANK>(p, 0, depth_max, n, g1, zero_32()))
			return true;
	}

	return false;
}

template <GroupType ForR>
bool Search::search_sol_xwing_body(problem& p, const int depth, const int depth_max, const Number num, Group* g1, const bitarray32 g2)
{
	auto cnt = g2.popcnt();
	if (cnt == depth && cnt > 0) {
		// 消す候補があるか
		auto flgValid = false;
		auto cand = p.cand_bb(num);
		for (int i = 0; i < depth; ++i) cand &= ~group_bb(g1[i]);
		auto tmp = g2;
		do {
			if ((cand & group_bb((Group)tmp.pop())).isnot_empty()) {
				flgValid = true;
				break;
			}
		} while (tmp.isnot_empty());

		if (flgValid) {
			bitarray32 g1list = zero_32();
			for (int i = 0; i < depth; ++i)
				g1list |= each_32(g1[i]);

			add_sol(p, new sol_xwing(num, g1list, g2));
			return true;
		}
	}
	if (cnt > depth_max) return false;
	if (depth == depth_max) return false;

	for (g1[depth] = depth == 0 ? gr_zero<ForR>() : (g1[depth - 1] + 1); g1[depth] < gr_nb<ForR>(); ++g1[depth]) {
		auto tmp = p.cand_bb(num) & group_bb(g1[depth]);
		if (tmp.isnot_empty()) {
			auto newg2 = g2;
			do newg2 |= each_32(group_of<ForR == GRT_FILE ? GRT_RANK : GRT_FILE>(tmp.pop())); while (tmp.isnot_empty());
			if (search_sol_xwing_body<ForR>(p, depth + 1, depth_max, num, g1, newg2))
				return true;
		}
	}

	return false;
}

// pro を元にして問題を作成
void Search::make_problem(const problem& pro)
{
	// 一度に求める解の個数の上限
	CSI ANSWER_MAX = 10;

	flg_exit = false;
	auto p = pro;
	bool flg_search = true;

	// 処理時間を計測
	chrono::system_clock::time_point start, end;
	double solve_time = 0, ans0_time = 0, ans1_time = 0, ans2_time = 0;

	for (;;)
	{
		// 前の解が再利用できないなら解く
		if (flg_search) {
			start = chrono::system_clock::now();
			// p の解を求める
			solve_all(p, false, true, ANSWER_MAX);
			end = chrono::system_clock::now();
			solve_time += (double)(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
			if (flg_exit) {
				cout << "stopped" << endl;
				return;
			}
		}

		if (ans.size() == 0) {
			start = chrono::system_clock::now();
			// どこかの数字を消す
			auto sq = (~p.empty_bb()).peek_random(mt);
			p.erase(sq);
			flg_search = true;
			end = chrono::system_clock::now();
			ans0_time += (double)(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
		}
		else if (ans.size() == 1) {
			start = chrono::system_clock::now();
			// どこかの数字を消しても解が 1つのままなら消す
			auto flg_finished = true;
			auto bb = ~p.empty_bb();
			do {
				auto sq = bb.pop_random(mt);
				auto n = p.at(sq);
				p.erase(sq);
				solve_all(p, false, false, 2);
				if (ans.size() == 1) {
					flg_finished = false;
					flg_search = true;
					break;
				}
				p.set(sq, n);
			} while (bb.isnot_empty());
			end = chrono::system_clock::now();
			ans1_time += (double)(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
			if (flg_finished) break;
		}
		else {
			start = chrono::system_clock::now();
			// ans のうち，増やすと最も解答数が少なくなる数字を増やす

			// sq に n が入る解答の数
			int count[SQ_NB][NUM_NB];
			memset(count, 0, sizeof(count));

			for (auto a : ans)
				for (auto sq : Square())
					count[sq][a->at(sq)]++;

			int min = INT_MAX;
			int min_count = 0;
			for (auto sq : Square()) {
				for (auto n : Number()) {
					if (count[sq][n] == min) min_count++;
					else if (FORMULA(0 <, count[sq][n], < min)) {
						min_count = 1;
						min = count[sq][n];
					}
				}
			}

			// [0, min_count) 番目を採用
			int index = mt() % min_count;
			
			for (auto sq : Square()) {
				for (auto n : Number()) {
					if (count[sq][n] == min && index-- == 0) {
						p.set(sq, n);
						ASSERT(is_ok(p));
						
						if (ans.size() < ANSWER_MAX) {
							auto cpy = ans;
							ans.clear();
							for (auto b : cpy) {
								if (b->num_bb(n).at(sq))
									ans.push_back(b);
								else
									delete b;
							}
							flg_search = false;
						}
						else {
							flg_search = true;
						}

						goto LOOPEND;
					}
				}
			}

		LOOPEND:;
			end = chrono::system_clock::now();
			ans2_time += (double)(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
		}
	}

	// 完成
	cout << "made " << p.tostring() << endl;
	cout << "info time solve " << (int)solve_time <<
		" ans0 " << (int)ans0_time << " ans1 " << (int)ans1_time << " ans2 " << (int)ans2_time << endl;
}
