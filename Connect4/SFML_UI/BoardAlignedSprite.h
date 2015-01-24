#pragma once
#include <SFML/Graphics.hpp>
#include "SFMLOperations.h"
#include "ResourceManager.h"
#include "Grid.h"


class BoardAlignedSprite :
	public sf::Sprite
{
public:
	BoardAlignedSprite(Grid* g);
	BoardAlignedSprite(Grid* g, std::string image);
	~BoardAlignedSprite(void);

	inline virtual void SetImage(const sf::Image& img) { Sprite::SetImage(img); SetCenter(sfmlop::divide(GetSize(), 2)); }

	void moveOnSquare(sf::Vector2i square, bool instant = true);

	virtual void update(float elapsedTime);


private:
	Grid* m_grid;
	sf::Vector2f m_goal;
};
