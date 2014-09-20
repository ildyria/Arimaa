#include "BoardAlignedSprite.h"

BoardAlignedSprite::BoardAlignedSprite() : sf::Sprite()
{
}

BoardAlignedSprite::BoardAlignedSprite(std::string image) : sf::Sprite(*ResourceManager::getImage(image))
{
	SetCenter(sfmlop::divide(GetSize(), 2));
}

BoardAlignedSprite::~BoardAlignedSprite(void)
{
	
}
