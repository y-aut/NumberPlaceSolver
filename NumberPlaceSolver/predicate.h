#pragma once

#include <typeinfo>

#include "problem.h"
#include "solution.h"

// 問題に対する制約
class predicate
{
public:
	// 問題を解くことが必要か
	virtual bool need_sol() const { return false; }

	// 制約を満たしているか
	virtual bool meet(const problem& p, const vector<const solution*>& sol) const = 0;
};

// 特定の解法を用いる
class pre_solution : public predicate
{
private:
	const type_info* ty;

public:
	pre_solution(const type_info* _ty) : ty(_ty) {}

	bool need_sol() const override { return true; }

	bool meet(const problem& p, const vector<const solution*>& sol) const override {
		for (auto s : sol)
			if (typeid(*s) == *ty) return true;
		return false;
	}
};
