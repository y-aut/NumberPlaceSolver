#pragma once

#include "game.h"
#include "problem.h"

// ���ɑ΂��ĕs�ρi���̌����ς��Ȃ��j�ȕϊ�
class trans
{
protected:
	virtual File apply(const File f) const { return f; }
	virtual Rank apply(const Rank r) const { return r; }

public:
	virtual Square apply(const Square sq) const { return apply(file_of(sq)) | apply(rank_of(sq)); }
	virtual bitboard apply(bitboard) const;
	virtual void apply(board&) const;
	virtual void apply(problem&) const;
	virtual trans reverse() const { return *this; }
};

enum MirrorEnum { MIRROR_X = 1, MIRROR_Y = 2, MIRROR_XY = MIRROR_X | MIRROR_Y };

// �Ώ̈ړ�
class mirror : public trans
{
private:
	MirrorEnum type;

public:
	mirror(MirrorEnum _type) : type(_type) {}

	File apply(const File f) const override { return (type & MIRROR_X) != 0 ? FILE_NB - f - 1 : f; }
	Rank apply(const Rank r) const override { return (type & MIRROR_Y) != 0 ? RANK_NB - r - 1 : r; }
};

enum RotateEnum { ROTATE_CLOCK, ROTATE_ANTI_CLOCK, };

// ��]
class rotate : public trans
{
private:
	RotateEnum type;

public:
	rotate(RotateEnum _type) : type(_type) {}

	Square apply(const Square sq) const override;
	trans reverse() const override { return rotate(type == ROTATE_CLOCK ? ROTATE_ANTI_CLOCK : ROTATE_CLOCK); }
};

// �u��
// ���Ƃ̖��̓u���b�N���ł̍s�C��̒u���C�u���b�N���Ƃ̍s�C��̒u���ɑ΂��ĕs��
class permutation : public trans
{
public:
	// GRT(g1) == GRT(g2)
	// { BLOCK_00, BLOCK_20 } �Ȃ� 1~3�s�ڂ� 7~9�s�ڂ̓���ւ�
	Group g1, g2;

private:
	permutation(Group _g1, Group _g2) : g1(_g1), g2(_g2) {}

	File apply(const File f) const override;
	Rank apply(const Rank r) const override;
};
