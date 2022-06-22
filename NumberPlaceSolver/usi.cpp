#include <iostream>
#include <sstream>
#include <future>
#include <Windows.h>
#include <regex>

#include "usi.h"
#include "bitboard.h"
#include "bitarray32.h"
#include "search.h"
#include "predicate.h"

using namespace std;

problem p;
future<void> best;
vector<const predicate*> pre;

int USI::loop()
{
	string line, cmd;
	vector<string> cmds;
	stringstream ss;

	setvbuf(stdout, NULL, _IONBF, 0);

	bitboard::ready();
	bitarray32::ready();

	Number num[SQ_NB];
	for (auto sq : Square()) num[sq] = NUM_NONE;
	p = problem(num);

	while (1)
	{
		do {
			Sleep(10);
			getline(cin, line);
		} while (line == "");
		// スペースで区切る
		ss = stringstream{ line };
		cmds.clear();
		while (getline(ss, cmd, ' ')) {
			cmds.push_back(cmd);
		}

		if (cmds[0].size() > 2 && cmds[0].substr(0, 2) == ">:")
			cmds[0] = cmds[0].substr(2, cmds[0].size() - 2);
		if (cmds[0] == "usi") usi_cmd();
		else if (cmds[0] == "setoption" || cmds[0] == "set") setoption_cmd(cmds);
		else if (cmds[0] == "problem" || cmds[0] == "p") problem_cmd(cmds);
		else if (cmds[0] == "solve" || cmds[0] == "s") solve_cmd(cmds);
		else if (cmds[0] == "make" || cmds[0] == "m") make_cmd(cmds);
		else if (cmds[0] == "stop") stop_cmd();
		else if (cmds[0] == "quit") { quit_cmd(); break; }
		else cout << "info string unsupported command:" << cmds[0] << endl;
	}

	return 0;
}

void USI::usi_cmd()
{
	cout << "NumberPlaceSolver_usiok" << endl;
}

void USI::setoption_cmd(const vector<string> cmds)
{
	try
	{
		if (cmds.size() < 2) throw exception();
		if (cmds[1] == "predicate" || cmds[1] == "p") {
			if (cmds.size() < 3) throw exception();
			if (cmds[2] == "add" || cmds[2] == "a") {
				if (cmds.size() < 4) throw exception();
				if (cmds[3] == "xwing") {
					pre.push_back(new pre_solution(&typeid(sol_xwing)));
				}
				else if (cmds[3] == "xychain") {
					pre.push_back(new pre_solution(&typeid(sol_xychain)));
				}
				else if (cmds[3] == "xychain_disc") {
					pre.push_back(new pre_solution(&typeid(sol_xychain_disc)));
				}
				else throw exception();
			}
			else if (cmds[2] == "clear" || cmds[2] == "c") {
				for (auto p : pre) delete p;
				pre.clear();
			}
			else throw exception();
		}
		else throw exception();
	}
	catch (const std::exception&)
	{
		cout << "info string illegal expression." << endl;
	}
}

void USI::problem_cmd(const vector<string> cmds)
{
	if (cmds.size() < 2 || cmds[1].size() < SQ_NB) {
		cout << "info string illegal expression." << endl;
		return;
	}
	Number num[SQ_NB];
	for (Square sq : Square())
		num[sq] = FORMULA('1' <=, cmds[1][sq], <= '9') ? (Number)(cmds[1][sq] - '1' + NUM_ZERO) : NUM_NONE;
	p = problem(num);
}

void USI::solve_cmd(const vector<string> cmds)
{
	auto cpy = p;
	if (cmds.size() >= 2 && cmds[1] == "all")
		best = async(launch::async, [] { return Search::solve_all(p, true); });
	else
		best = async(launch::async, [] { return Search::solve(p, true); });
}

void USI::make_cmd(const vector<string> cmds)
{
	best = async(launch::async, [] { return Search::make_problem_pre(p, pre); });
}

void USI::stop_cmd()
{
	Search::flg_exit = true;
	try {
		best.get();
	}
	catch (exception ex) {}
}

void USI::quit_cmd()
{
	stop_cmd();
}

