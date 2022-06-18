#include "board.h"

string board::tostring() const
{
	ASSERT(is_ok(*this));
	string ans = string(CELL_CNT, '.');
	for (auto n : Number()) {
		auto bb = num_bb(n);
		while (bb.isnot_empty()) {
			auto sq = bb.pop();
			ans[sq] = to_char(n);
		}
	}
	return ans;
}

bool is_ok(const board& b)
{
	for (auto n : Number()) {
		if (!b.num_bb(n).is_ok()) return false;
		for (auto g : Group()) {
			if ((b.num_bb(n) & group_bb(g)).popcnt() > 1)
				return false;
		}
	}
	return true;
}
