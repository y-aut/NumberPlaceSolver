#pragma once

#include <string>
#include "game.h"
#include "problem.h"
#include "bitarray32.h"

using namespace std;

class solution
{
public:
	virtual string tostring() const = 0;
	virtual void apply(problem &p) const = 0;
};

class sol_alone_num : public solution
{
	// sqに入る数字がnumのみ
	Square sq;
	Number num;

public:
	sol_alone_num(const Square _sq, const Number _num) : sq(_sq), num(_num) {}
	string tostring() const override { return "alonenum " + to_string(sq) + " " + to_string(num); }
	void apply(problem& p) const override { p.set(sq, num); }
};

class sol_alone_num_in_group : public solution
{
	// group内でnumが入るのはsqのみ
	Square sq;
	Number num;
	Group g;

public:
	sol_alone_num_in_group(const Square _sq, const Number _num, const Group _g)
		: sq(_sq), num(_num), g(_g) {}
	string tostring() const override { return "alone " + to_string(sq) + " " + to_string(num) + " " + to_string(g); }
	void apply(problem& p) const override { p.set(sq, num); }
};

class sol_occupy : public solution
{
	// group内でsqsにはnumsしか入らない
	bitboard sqs;
	bitarray32 nums; // List of num
	Group g;

public:
	sol_occupy(const bitboard _sqs, const bitarray32 _nums, const Group _g)
		: sqs(_sqs), nums(_nums), g(_g) {}
	string tostring() const override;
	void apply(problem& p) const override;
};

class sol_reserve : public solution
{
	// {g1, g2} == {!block, block}
	// g1で，numはsqsにしか入らないので，g2の残りのマスにはnumは入らない
	bitboard sqs;
	Number num;
	Group g1;
	Group g2;

public:
	sol_reserve(const bitboard _sqs, const Number _num, const Group _g1, const Group _g2)
		: sqs(_sqs), num(_num), g1(_g1), g2(_g2) {}
	string tostring() const override;
	void apply(problem& p) const override;
};

class sol_xwing : public solution
{
	// {g1, g2} == {File, Rank}
	// g1で，numが入るのはg2のどれかなので，g2の残りのマスにはnumは入らない
	Number num;
	bitarray32 g1; // List of Group
	bitarray32 g2; // List of Group

public:
	sol_xwing(const Number _num, const bitarray32 _g1, const bitarray32 _g2)
		: num(_num), g1(_g1), g2(_g2) {}
	string tostring() const override;
	void apply(problem& p) const override;
};

class sol_xychain : public solution
{
	// chain_num[i] には chain[i - 1] と chain[i] に共通する候補が入っている
	// chain[i], chain[i + 1] に共通するグループの他のマスには，
	// chain_num[i + 1] を入れることはできない
	// chain の最後のマスは chain[0] と同じグループにある
	vector<Square> chain;
	vector<Number> chain_num;

public:
	sol_xychain(const Square c[], const Number c_num[], const int c_size) {
		for (int i = 0; i < c_size; ++i) {
			chain.push_back(c[i]);
			chain_num.push_back(c_num[i]);
		}
	}
	string tostring() const override;
	void apply(problem& p) const override;
};

class sol_xychain_disc : public solution
{
	// chain_num[i] には chain[i - 1] と chain[i] に共通する候補が入っている
	// chain[0] と chain.last の共通する領域には、chain_num[0] を入れることはできない
	// chain の最後のマスは chain[0] と同じグループにある
	vector<Square> chain;
	vector<Number> chain_num;

public:
	sol_xychain_disc(const Square c[], const Number c_num[], const int c_size) {
		for (int i = 0; i < c_size; ++i) {
			chain.push_back(c[i]);
			chain_num.push_back(c_num[i]);
		}
	}
	string tostring() const override;
	void apply(problem& p) const override;
};

class sol_simple_chain : public solution
{
	// chain.size() は偶数
	// 強リンク，弱リンク，... ，強リンク
	// chain[0] と chain.last の共通領域には，num を入れることはできない
	Number num;
	vector<Square> chain;

public:
	sol_simple_chain(const Number n, const Square c[], const int c_size) : num(n) {
		for (int i = 0; i < c_size; ++i)
			chain.push_back(c[i]);
	}
	string tostring() const override;
	void apply(problem& p) const override;
};

class sol_hamada : public solution
{
	// chain.size() は奇数
	// 弱リンク，強リンク，... ，強リンク
	// chain[0] には，num を入れることはできない
	// なぜなら，grp 内で num を入れられる場所がなくなるから
	Number num;
	Group grp;
	vector<Square> chain;

public:
	sol_hamada(const Number n, const Group g, const Square c[], const int c_size) : num(n), grp(g) {
		for (int i = 0; i < c_size; ++i)
			chain.push_back(c[i]);
	}
	string tostring() const override;
	void apply(problem& p) const override;
};