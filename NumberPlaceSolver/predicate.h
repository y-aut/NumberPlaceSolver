#pragma once

#include <typeinfo>

#include "problem.h"
#include "solution.h"

// ���ɑ΂��鐧��
class predicate
{
public:
	// �����������Ƃ��K�v��
	virtual bool need_sol() const { return false; }

	// ����𖞂����Ă��邩
	virtual bool meet(const problem& p, const bool solved, const vector<const solution*>& sol) const = 0;
};

// �t
class pre_reverse : public predicate
{
private:
	const predicate* pre;

public:
	pre_reverse(const predicate* p) : pre(p) {}

	bool need_sol() const override { return pre->need_sol(); }

	bool meet(const problem& p, const bool solved, const vector<const solution*>& sol) const override {
		return !pre->meet(p, solved, sol);
	}
};

// ������
class pre_solvable : public predicate
{
public:
	pre_solvable() {}

	bool need_sol() const override { return true; }

	bool meet(const problem& p, const bool solved, const vector<const solution*>& sol) const override {
		return solved;
	}
};

// ����̉�@��p����
class pre_solution : public predicate
{
private:
	const type_info* ty;

public:
	pre_solution(const type_info* _ty) : ty(_ty) {}

	bool need_sol() const override { return true; }

	bool meet(const problem& p, const bool solved, const vector<const solution*>& sol) const override {
		for (auto s : sol)
			if (typeid(*s) == *ty) return true;
		return false;
	}
};
