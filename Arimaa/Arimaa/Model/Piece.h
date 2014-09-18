#pragma once

enum PieceType {RABBIT, CAT, DOG, HORSE, CAMEL, ELEPHANT, NB_PIECES};
enum Color {GOLD, SILVER};

class Piece
{
public:
	Piece(PieceType type = RABBIT, Color color = GOLD);
	~Piece(void);

	inline PieceType getType() const { return m_type; }
	inline Color getColor() const { return m_color; }

private:
	PieceType m_type;
	Color m_color;

};

inline bool operator< (const Piece& p1, const Piece& p2) { return (p1.getType() < p2.getType()); }
inline bool operator> (const Piece& p1, const Piece& p2) { return (p1.getType() > p2.getType()); }
inline bool operator<= (const Piece& p1, const Piece& p2) { return (p1.getType() <= p2.getType()); }
inline bool operator>= (const Piece& p1, const Piece& p2) { return (p1.getType() >= p2.getType()); }