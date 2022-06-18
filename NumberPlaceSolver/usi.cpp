#include <iostream>
#include <sstream>
#include <future>
#include <Windows.h>
#include <regex>

#include "usi.h"
#include "bitboard.h"
#include "bitarray32.h"
#include "search.h"

using namespace std;

problem p;
future<void> best;

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
	best = async(launch::async, [] { return Search::make_problem(p); });
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

