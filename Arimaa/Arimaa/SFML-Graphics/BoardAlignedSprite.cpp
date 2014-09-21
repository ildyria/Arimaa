#include "BoardAlignedSprite.h"

#define BOARD_SPEED 3000

BoardAlignedSprite::BoardAlignedSprite() : sf::Sprite(), m_goal(sf::Vector2f(-1,-1))
{
}

BoardAlignedSprite::BoardAlignedSprite(std::string image) : sf::Sprite(*ResourceManager::getImage(image)), m_goal(sf::Vector2f(-1,-1))
{
	SetCenter(sfmlop::divide(GetSize(), 2));
}

BoardAlignedSprite::~BoardAlignedSprite(void)
{
	
}

void BoardAlignedSprite::moveOnSquare(sf::Vector2i square, bool instant)
{
	if(isOnBoard(square))
	{
		if(instant || m_goal.x < 0) //if the sprite isn't on the board yet, we put it on the board. If instant, we warp it instantly
			SetPosition( toPixels(square) );
		m_goal = toPixels(square);
	}
}

void BoardAlignedSprite::update(float elapsedTime)
{
	if(m_goal.x >= 0 && GetPosition() != m_goal) //if the goal is reachable and not reached, go towards it
	{
		sf::Vector2f difference = m_goal - GetPosition();
		float squaredDist = sfmlop::getSquaredNorm(difference);
		float movement = BOARD_SPEED*elapsedTime;
		if(squaredDist <= movement*movement) //if we can reach the goal this turn, reach it
			SetPosition(m_goal);
		else //else go towards it
		{
			sf::Vector2f offset = sfmlop::multiply(sfmlop::normalize(difference), movement);
			Move(offset);
		}
	}
}