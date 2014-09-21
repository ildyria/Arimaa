#pragma once
#include "../StdAfx.h"
#include <SFML/Graphics.hpp>
#include "SFMLOperations.h"
#include "ResourceManager.h"
#include "ConfigOptions.h"

class TurnSign
{
public:
	TurnSign(void);
	~TurnSign(void);

	void loadAssets();
	void unloadAssets();

	void update(float elapsedTime);
	void draw(sf::RenderWindow &app);

	void activate(bool goldTurn);
	bool isActive() const;

private:
	sf::Sprite m_nextTurnSprite;
	sf::Sprite m_glowSprite;
	float m_appearanceTimer;
	bool m_activated;
	bool m_appearing;

	void reset();
};
