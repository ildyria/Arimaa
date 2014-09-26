#pragma once
#include "BoardAlignedSprite.h"

#define HIGHLIGHT_SPRITE "Highlight.png"

class Highlighter
{
public:
	Highlighter(void);
	~Highlighter(void);

	inline void loadAssets() { ResourceManager::getImage(HIGHLIGHT_SPRITE); }
	inline void unloadAssets() { ResourceManager::unloadImage(HIGHLIGHT_SPRITE); }

	inline void draw(sf::RenderWindow &app) { for(int i = 0; i < m_nbHighlights; ++i) app.Draw(m_sprites[i]); }

	void highlight(sf::Vector2i square);
	inline void highlight(std::vector<sf::Vector2i> squares) { for(unsigned int i = 0; i < squares.size(); ++i) highlight(squares[i]); }
	inline void setHighlights(std::vector<sf::Vector2i> squares) { removeHighlights(); highlight(squares); }
	inline void removeHighlights() { m_nbHighlights = 0; }
	bool isHighlighted(sf::Vector2i square);

private:
	std::vector<BoardAlignedSprite> m_sprites;
	int m_nbHighlights;
};
