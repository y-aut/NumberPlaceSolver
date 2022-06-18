#include "solution.h"

string sol_occupy::tostring() const
{
    auto bb = sqs;
    string str = "occupy " + to_string(g) + " num " + to_string(nums.popcnt()) + " ";
    auto cpy = nums;
    do str += to_string((Number)cpy.pop()) + " "; while (cpy.isnot_empty());
    str += "sq " + to_string(bb.popcnt()) + " ";
    while (bb.isnot_empty()) str += to_string(bb.pop()) + " ";
    return str.substr(0, str.size() - 1);
}

void sol_occupy::apply(problem& p) const
{
    auto mask = sqs | ~group_bb(g);
    auto cpy = nums;
    do *p.cand_bb_ptr((Number)cpy.pop()) &= mask; while (cpy.isnot_empty());
}

string sol_reserve::tostring() const
{
    auto bb = sqs;
    string str = "reserve " + to_string(g1) + " " + to_string(g2) + " num " + to_string(num) + " sq "
        + to_string(bb.popcnt()) + " ";
    while (bb.isnot_empty()) str += to_string(bb.pop()) + " ";
    return str.substr(0, str.size() - 1);
}

void sol_reserve::apply(problem& p) const
{
    *p.cand_bb_ptr(num) &= group_bb(g1) | ~group_bb(g2);
}

string sol_xwing::tostring() const
{
    auto g1_ = g1; auto g2_ = g2;
    string str = "xwing " + to_string(num) + " src " + to_string(g1.popcnt()) + " ";
    do str += to_string((Group)g1_.pop()) + " "; while (g1_.isnot_empty());
    str += "tar " + to_string(g2.popcnt()) + " ";
    do str += to_string((Group)g2_.pop()) + " "; while (g2_.isnot_empty());
    return str.substr(0, str.size() - 1);
}

void sol_xwing::apply(problem& p) const
{
    auto g1_ = g1; auto g2_ = g2;
    auto mask = all_bb();
    do mask ^= group_bb((Group)g2_.pop()); while (g2_.isnot_empty());
    do mask |= group_bb((Group)g1_.pop()); while (g1_.isnot_empty());
    *p.cand_bb_ptr(num) &= mask;
}
