#include "TurnSign.h"

#define APPEARANCE_SPD 4
#define SHINE_SPEED 2
#define GLOW_SPEED ConfigOptions::nativeWidth()*SHINE_SPEED

#define SIGN_HEIGHT 112

TurnSign::TurnSign(void) : m_activated(false)
{
	m_nextTurnSprite.SetPosition(sf::Vector2f(0, ConfigOptions::getNativeCenter().y));
	m_nextTurnSprite.SetCenter(0,SIGN_HEIGHT/2);
	m_glowSprite.SetPosition(sf::Vector2f(0, ConfigOptions::getNativeCenter().y));
	m_glowSprite.SetCenter(0,SIGN_HEIGHT/2);
}

TurnSign::~TurnSign(void)
{
}

void TurnSign::loadAssets()
{
	if (m_nextTurnSprite.GetImage() == nullptr)
		m_nextTurnSprite.SetImage(*ResourceManager::getImage("Next_Turn.png"));
	if (m_glowSprite.GetImage() == nullptr)
		m_glowSprite.SetImage(*ResourceManager::getImage("Turn_Glow.png"));
	reset();
}

void TurnSign::unloadAssets()
{
	ResourceManager::unloadImage("Next_Turn.png");
	ResourceManager::unloadImage("Turn_Glow.png");
}

void TurnSign::update(float elapsedTime)
{
	if(!m_activated)
		return;

	if(!m_appearing)
	{
		m_appearanceTimer -= elapsedTime*APPEARANCE_SPD;
		if(m_appearanceTimer < 0)
			reset();
		else
		{
			m_nextTurnSprite.SetScaleY(m_appearanceTimer);
			m_nextTurnSprite.SetColor(sf::Color(255,255,255,(int)(255*m_appearanceTimer)));
		}
	}
	else if(m_appearanceTimer < 1)
	{
		m_appearanceTimer += elapsedTime*APPEARANCE_SPD;
		if(m_appearanceTimer > 1)
			m_appearanceTimer = 1;

		m_nextTurnSprite.SetScaleY(m_appearanceTimer);
		m_nextTurnSprite.SetColor(sf::Color(255,255,255,(int)(255*m_appearanceTimer)));
	}
	else if(m_glowSprite.GetPosition().x < ConfigOptions::nativeWidth())
		m_glowSprite.Move(sf::Vector2f(GLOW_SPEED*elapsedTime,0));
	else
		m_appearing = false;
}

void TurnSign::activate(bool goldTurn)
{
	if(!m_activated)
	{
		m_activated = true;
		int width = m_nextTurnSprite.GetImage()->GetWidth();
		int height = m_nextTurnSprite.GetImage()->GetHeight();
		if(goldTurn)
			m_nextTurnSprite.SetSubRect(sf::IntRect(0, 0, width, height/2)); //top sprite
		else
			m_nextTurnSprite.SetSubRect(sf::IntRect(0, height/2, width, height)); //bottom sprite		
	}
}

bool TurnSign::isActive() const
{
	return m_activated;
}

void TurnSign::draw(sf::RenderWindow &app)
{
	if(m_activated)
	{
		app.Draw(m_nextTurnSprite);
		if(m_appearanceTimer == 1)
			app.Draw(m_glowSprite);
	}
}

void TurnSign::reset()
{
	m_appearanceTimer = 0;
	m_activated = false;
	m_appearing = true;
	m_glowSprite.SetPosition(-m_glowSprite.GetSize().x, m_nextTurnSprite.GetPosition().y);
	m_nextTurnSprite.SetScaleY(1);
	m_nextTurnSprite.SetColor(sf::Color(255,255,255,0));
}