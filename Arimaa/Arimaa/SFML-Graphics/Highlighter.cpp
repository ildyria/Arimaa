#include "Highlighter.h"

Highlighter::Highlighter(void) : m_nbHighlights(0)
{
}

Highlighter::~Highlighter(void)
{
}

void Highlighter::highlight(sf::Vector2i square)
{
	if(m_nbHighlights >= (int) m_sprites.size()) //no more space in m_sprites
		m_sprites.push_back(BoardAlignedSprite(HIGHLIGHT_SPRITE));

	m_sprites[m_nbHighlights].moveOnSquare(square); //moving the highlight on the right square
	m_nbHighlights++;
}

bool Highlighter::isHighlighted(sf::Vector2i square)
{
	sf::Vector2f pos = BoardAlignedSprite::toPixels(square);
	for(int i = 0; i < m_nbHighlights; ++i)
		if(pos == m_sprites[i].GetPosition())
			return true;
	return false;
}