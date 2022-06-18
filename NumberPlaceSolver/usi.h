#pragma once

#include <vector>
#include <string>

#include "problem.h"

using namespace std;

namespace USI
{
	// 応答部本体
	int loop();

	// コマンド処理
	void usi_cmd();
	void problem_cmd(const vector<string>);
	void solve_cmd(const vector<string>);
	void make_cmd(const vector<string>);
	void stop_cmd();
	void quit_cmd();
}

/*	---- USIプロトコル ----

	- usi		-- NumberPlaceSolver_usiok

	- problem 1..5..2...	（左上から下方向に）

	- solve		-- answer 13256...

	- stop

	- quit

*/