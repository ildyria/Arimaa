#include "Piece.h"

Piece::Piece(PieceType type, Color color) : m_type(type), m_color(color)
{
}

Piece::~Piece(void)
{
}

char Piece::toChar() const
{
	if(m_color == GOLD)
		return 'a' + m_type;
	else
		return 'A' + m_type;
}

Piece Piece::fromChar(char c)
{
	Piece p;
	if(c >= 'a')
	{
		p.m_color = GOLD;
		c -= 'a';
	}
	else
	{
		p.m_color = SILVER;
		c -= 'A';
	}
	p.m_type = (PieceType) c;
	return p;
}