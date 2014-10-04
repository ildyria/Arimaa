#pragma once

/**
 *  \brief The different types of pieces available.
 *  \details NB_PIECES is used to store the number of piece types.
 */
enum PieceType {RABBIT, CAT, DOG, HORSE, CAMEL, ELEPHANT, NB_PIECES};
/**
 *  \brief The different player colors available.
 *  \details NB_PLAYERS is used to store the number players.
 */
enum Color {GOLD, SILVER, NB_PLAYERS};

/**
 *  \brief A class representing game pieces.
 */
class Piece
{
public:
	/**
	 *  \brief Basic constructor for Piece.
	 *  
	 *  \param type The type of the piece.
	 *  \param color The color of the piece's owner.
	 */
	Piece(PieceType type = RABBIT, Color color = GOLD);
	~Piece(void);

	/**
	 *  \brief Returns the type of the piece.
	 *  
	 *  \return The type of the piece.
	 */
	inline PieceType getType() const { return m_type; }
	/**
	 *  \brief Returns the color of the piece's owner.
	 *  
	 *  \return The color of the piece's owner.
	 */
	inline Color getColor() const { return m_color; }

	/**
	 *  \brief Converts the Piece into a char, useful when saving Game data.
	 *  
	 *  \return The char equivalent of the piece.
	 *  
	 *  \details Gold pieces are represented by lowercase letters, and silver pieces by uppercase letters.
	 *  Rabbits correspond to 'a', cats to 'b', etc.
	 */
	char toChar() const;
	/**
	 *  \brief Converts a char into a Piece, useful when loading Game data.
	 *  
	 *  \param c The char to convert.
	 *  \return The piece corresponding to c.
	 *  
	 *  \details Gold pieces are represented by lowercase letters, and silver pieces by uppercase letters.
	 *  Rabbits correspond to 'a', cats to 'b', etc.
	 */
	static Piece fromChar(char c);

private:
	PieceType m_type;
	Color m_color;

};

inline bool operator< (const Piece& p1, const Piece& p2) { return (p1.getType() < p2.getType()); }
inline bool operator> (const Piece& p1, const Piece& p2) { return (p1.getType() > p2.getType()); }
inline bool operator<= (const Piece& p1, const Piece& p2) { return (p1.getType() <= p2.getType()); }
inline bool operator>= (const Piece& p1, const Piece& p2) { return (p1.getType() >= p2.getType()); }