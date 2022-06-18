#pragma once

#include <vector>
#include <string>

#include "problem.h"

using namespace std;

namespace USI
{
	// �������{��
	int loop();

	// �R�}���h����
	void usi_cmd();
	void problem_cmd(const vector<string>);
	void solve_cmd(const vector<string>);
	void make_cmd(const vector<string>);
	void stop_cmd();
	void quit_cmd();
}

/*	---- USI�v���g�R�� ----

	- usi		-- NumberPlaceSolver_usiok

	- problem 1..5..2...	�i���ォ�牺�����Ɂj

	- solve		-- answer 13256...

	- stop

	- quit

*/